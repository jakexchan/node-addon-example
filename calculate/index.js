const calculate = require('bindings')('calculate');

console.log(calculate.plus(1, 1)); // 2
console.log(calculate.minus(10, 2)); // 8
