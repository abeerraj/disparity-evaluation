const path = require('path')
const utils = require('../common/utils')
const csv = require('../common/csv')
const _ = require('lodash')

function readResults(evalPath, cb) {
  csv.read(path.join(evalPath, 'mean_results.csv'), results => {
    csv.read(path.join(evalPath, 'mean_runtime.csv'), runtime => {
      const data = _.merge(results, runtime).map(row => {
        return {
          algorithmId: row.algorithm,
          algorithm: utils.getAlgorithmName(row.algorithm),
          runtime: row.runtime.toFixed(0),
          meanPbmp: parseFloat(row.pbmpNoc1).toFixed(2),
          meanRmse: parseFloat(row.rmseNoc).toFixed(2)
        }
      })
      cb(data)
    })
  })
}

module.exports = function(req, res) {
  const dataset = req.params.dataset
  const sequence = req.params.sequence
  const evalPath = path.join(dataset, sequence, 'eval')

  readResults(evalPath, results => {
    const data = {
      selection: dataset + ' / ' + sequence,
      results: results
    }
    return res.render('result_list', data)
  })
}
