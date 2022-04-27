#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */
 
const wtf = require('./_common')
const fs = require('fs')
const path = require('path')
const shell = require('shelljs')

const config = require('./_config')

//  Set path to node for shelljs
const nodePath = (shell.which('node').toString())
shell.config.execPath = nodePath

//  Create then load the settings file
wtf.createSettings()
if(!wtf.checkSettings()) wtf.scriptError(
    `Unable to access 'settings.json'.  Verify you have access to write to this file`)

const settings = require('../settings')
