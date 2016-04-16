const fs = require('fs')
const path = require('path')
const config = require('../app/config')

module.exports = {
  controllers: injectControllers,
  getDirectories: getDirectories,
  getAlgorithmName: getAlgorithmName,
  countFiles: countFiles
}

function getAlgorithmName(algorithm) {
  if (algorithm == '0') return 'OpenCV - SGBM'
  if (algorithm == '1') return 'OpenCV - BM'
  if (algorithm == '2') return 'ELAS'
  if (algorithm == '3') return 'MRF - ICM'
  if (algorithm == '4') return 'MRF - GC Expansion'
  if (algorithm == '5') return 'MRF - GC Swap'
  if (algorithm == '6') return 'MRF - BP TRWS'
  if (algorithm == '7') return 'MRF - BP BPS'
  if (algorithm == '8') return 'MRF - BP BPM'
  if (algorithm == '9') return 'OpenCV - Simple BM'
  if (algorithm == '10') return 'SNSM'
  if (algorithm == '11') return 'SNSM - Spatiotemporal - unweighted'
  if (algorithm == '12') return 'SNSM - Spatiotemporal - weighted'
  return ''
}

function sortDirectories(a, b) {
  a = a.toLowerCase()
  b = b.toLowerCase()
  if (a > b) return 1
  if (a < b) return -1
  return 0
}

function countFiles(p) {
  const filesPath = path.join(config.datasets, p)
  const files = fs.readdirSync(filesPath)
  return files.length
}

function getDirectories(p) {
  return fs.readdirSync(path.join(config.datasets, p))
    .filter(function(f) {
      return fs.statSync(path.join(config.datasets, p, f)).isDirectory()
    })
    .sort(sortDirectories)
}

function isJS(f) {
  return f.indexOf('.js') > -1
}

function injectControllers() {
  var controllers = {}
  var p = __dirname + '/../controllers'
  fs.readdirSync(p)
    .filter(isJS)
    .forEach(function (f) {
      controllers[f.split('.')[0]] = require(p + '/' + f)
    })
  return controllers
}
