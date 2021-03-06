#!/usr/bin/env python
#
# Copyright (c) 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import collections
import copy
import json
import optparse
import os
import pipes
import subprocess
import sys

import bb_utils

sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
from pylib import buildbot_report

CHROME_SRC = os.path.abspath(
    os.path.join(os.path.dirname(__file__), '..', '..', '..'))

GLOBAL_SLAVE_PROPS = {}

BotConfig = collections.namedtuple(
    'BotConfig', ['bot_id', 'host_opts', 'test_obj', 'slave_props'])
TestConfig = collections.namedtuple('Tests', ['tests', 'extra_args'])
Command = collections.namedtuple(
    'Command', ['step_name', 'command', 'testing_cmd'])

CommandToString = bb_utils.CommandToString


def GetCommands(options, bot_config):
  """Get a formatted list of commands.

  Args:
    options: Options object.
    bot_config: A BotConfig named tuple.
  Returns:
    list of Command objects.
  """
  slave_props = dict(GLOBAL_SLAVE_PROPS)
  if bot_config.slave_props:
    slave_props.update(bot_config.slave_props)

  slave_properties = json.dumps(slave_props)
  property_args = [
      '--factory-properties=%s' % json.dumps(options.factory_properties),
      '--build-properties=%s' % json.dumps(options.build_properties),
      '--slave-properties=%s' % slave_properties]

  commands = []
  def WrapWithBash(command):
    """Wrap a bash command string with envsetup scripts."""
    return ['bash', '-exc', '; '.join([
        '. build/android/buildbot/buildbot_functions.sh',
        'bb_baseline_setup %s --slave-properties=%s' % (
            CHROME_SRC, pipes.quote(slave_properties)),
        command])
    ]

  if bot_config.host_opts:
    host_cmd = (['build/android/buildbot/bb_host_steps.py'] +
                bot_config.host_opts + property_args)
    commands.append(Command(
        'Host steps',
        WrapWithBash(' '.join(map(pipes.quote, host_cmd))), host_cmd))

  test_obj = bot_config.test_obj
  if test_obj:
    run_test_cmd = [
        'build/android/buildbot/bb_device_steps.py', '--reboot'] + property_args
    for test in test_obj.tests:
      run_test_cmd.extend(['-f', test])
    if test_obj.extra_args:
      run_test_cmd.extend(test_obj.extra_args)
    commands.append(Command(
        'Run tests',
        WrapWithBash(' '.join(map(pipes.quote, run_test_cmd))), run_test_cmd))

  return commands


def GetBotStepMap():
  compile_opt = ['--compile']
  std_host_tests = ['--host-tests=check_webview_licenses,findbugs']
  std_build_opts = ['--compile', '--zip-build']
  std_test_opts = ['--extract-build']
  std_tests = ['ui', 'unit']
  flakiness_server = '--upload-to-flakiness-server'
  extra_gyp = 'extra_gyp_defines'

  def B(bot_id, bash_funs, test_obj=None, slave_props=None):
    return BotConfig(bot_id, bash_funs, test_obj, slave_props)

  def T(tests, extra_args=None):
    return TestConfig(tests, extra_args)

  bot_configs = [
      # Main builders
      B('main-builder-dbg', std_build_opts + std_host_tests),
      B('main-builder-rel', std_build_opts),
      B('main-clang-builder', compile_opt, slave_props={extra_gyp: 'clang=1'}),
      B('main-clobber', compile_opt),
      B('main-tests', std_test_opts, T(std_tests, [flakiness_server])),

      # Other waterfalls
      B('asan-builder-tests', compile_opt + ['--update-clang'],
        T(std_tests, ['--asan']), {extra_gyp: 'asan=1'}),
      B('chromedriver-fyi-tests-dbg', std_test_opts,
        T(['chromedriver'], ['--install=ChromiumTestShell'])),
      B('fyi-builder-dbg',
        std_build_opts + std_host_tests + ['--experimental']),
      B('fyi-builder-rel', std_build_opts + ['--experimental']),
      B('fyi-tests-dbg-ics-gn', compile_opt + [ '--experimental'],
        T(std_tests, ['--experimental', flakiness_server])),
      B('fyi-tests', std_test_opts,
        T(std_tests, ['--experimental', flakiness_server])),
      B('fyi-component-builder-tests-dbg', compile_opt,
        T(std_tests, ['--experimental', flakiness_server]),
        {extra_gyp: 'component=shared_library'}),
      B('perf-tests-rel', std_test_opts, T([], ['--install=ContentShell'])),
      B('webkit-latest-webkit-tests', std_test_opts,
        T(['webkit_layout', 'webkit'])),
      B('webkit-latest-contentshell', compile_opt, T(['webkit_layout'])),
      B('builder-unit-tests', compile_opt, T(['unit'])),

      # Generic builder config (for substring match).
      B('builder', std_build_opts),
  ]

  bot_map = dict((config.bot_id, config) for config in bot_configs)

  # These bots have identical configuration to ones defined earlier.
  copy_map = [
      ('lkgr-clobber', 'main-clobber'),
      ('try-builder-dbg', 'main-builder-dbg'),
      ('try-builder-rel', 'main-builder-rel'),
      ('try-clang-builder', 'main-clang-builder'),
      ('try-fyi-builder-dbg', 'fyi-builder-dbg'),
      ('try-tests', 'main-tests'),
      ('try-fyi-tests', 'fyi-tests'),
      ('webkit-latest-tests', 'main-tests'),
  ]
  for to_id, from_id in copy_map:
    assert to_id not in bot_map
    # pylint: disable=W0212
    bot_map[to_id] = copy.deepcopy(bot_map[from_id])._replace(bot_id=to_id)

    # Trybots do not upload to flakiness dashboard. They should be otherwise
    # identical in configuration to their trunk building counterparts.
    test_obj = bot_map[to_id].test_obj
    if to_id.startswith('try') and test_obj:
      extra_args = test_obj.extra_args
      if extra_args and flakiness_server in extra_args:
        extra_args.remove(flakiness_server)
  return bot_map


def main(argv):
  parser = optparse.OptionParser()

  def ConvertJson(option, _, value, parser):
    setattr(parser.values, option.dest, json.loads(value))

  parser.add_option('--build-properties', action='callback',
                    callback=ConvertJson, type='string', default={},
                    help='build properties in JSON format')
  parser.add_option('--factory-properties', action='callback',
                    callback=ConvertJson, type='string', default={},
                    help='factory properties in JSON format')
  parser.add_option('--bot-id', help='Specify bot id directly.')
  parser.add_option('--TESTING', action='store_true',
                    help='For testing: print, but do not run commands')
  options, args = parser.parse_args(argv[1:])
  if args:
    parser.error('Unused args: %s' % args)

  bot_id = options.bot_id or options.factory_properties.get('android_bot_id')
  if not bot_id:
    parser.error('A bot id must be specified through option or factory_props.')

  # Get a BotConfig object looking first for an exact bot-id match. If no exact
  # match, look for a bot-id which is a substring of the specified id.
  # This allows similar bots to have unique IDs, but to share config.
  # If multiple substring matches exist, pick the longest one.
  bot_map = GetBotStepMap()
  bot_config = bot_map.get(bot_id)
  if not bot_config:
    substring_matches = filter(lambda x: x in bot_id, bot_map.iterkeys())
    if substring_matches:
      max_id = max(substring_matches, key=len)
      print 'Using config from id="%s" (substring match).' % max_id
      bot_config = bot_map[max_id]
  if not bot_config:
    print 'Error: config for id="%s" cannot be inferred.' % bot_id
    return 1

  print 'Using config:', bot_config

  command_objs = GetCommands(options, bot_config)
  for command_obj in command_objs:
    print 'Will run:', CommandToString(command_obj.command)

  for command_obj in command_objs:
    if command_obj.step_name:
      buildbot_report.PrintNamedStep(command_obj.step_name)
    command = command_obj.command
    print CommandToString(command)
    sys.stdout.flush()
    env = None
    if options.TESTING:
      if not command_obj.testing_cmd:
        continue
      return_code = subprocess.call(
          command_obj.testing_cmd,
          cwd=CHROME_SRC,
          env=dict(os.environ, BUILDBOT_TESTING='1'))
    else:
      return_code = subprocess.call(command, cwd=CHROME_SRC, env=env)
    if return_code != 0:
      return return_code


if __name__ == '__main__':
  sys.exit(main(sys.argv))
