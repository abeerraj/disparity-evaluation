const utils = require('../common/utils')

String.prototype.capitalizeFirstLetter = function() {
    return this.charAt(0).toUpperCase() + this.slice(1)
}

function getDiminishEffect(foo) {
  const foos = foo.split('-')
  if (foos.length == 1) return '-'
  return foos[1].toUpperCase() + ' (' + foos[2] + ')'
}

module.exports = function(req, res) {
  var sequences = []
  utils.getDirectories('/').forEach(dataset => {
    utils.getDirectories('/' + dataset).forEach(sequence => {
      sequences.push({
        dataset: dataset.split('-')[0].capitalizeFirstLetter(),
        diminish: getDiminishEffect(dataset),
        sequence: sequence,
        folder: dataset
      })
    })
  })
  const data = {
    results: sequences
  }
  return res.render('index', data)
}
