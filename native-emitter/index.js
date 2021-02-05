const { EventEmitter } = require('events');
const { inherits } = require('util');
const { NativeEmitter } = require('bindings')('native_emitter');

inherits(NativeEmitter, EventEmitter);

const nativeEmitter = new NativeEmitter();

nativeEmitter.on('message', (msg) => {
  console.info('message: ', msg);
});

nativeEmitter.callMessage();
