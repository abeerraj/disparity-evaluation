const Converter = require('csvtojson').Converter
const fs = require('fs')
const path = require('path')
const config = require('../app/config.js')

module.exports.read = function(inputFile, cb) {
  const filePath = path.join(config.datasets, inputFile)
  const file = fs.readFileSync(filePath, 'utf8')
  const converter = new Converter({
    'delimiter': ';',
    'ignoreEmpty': 'true'
  })
  converter.fromString(file, (err, res) => {
    if (err) throw err
    return cb(res)
  })
}
