const utils = require('../common/utils')
const path = require('path')
const csv = require('../common/csv')
const _ = require('lodash')

module.exports = function(req, res) {
  const dataset = req.params.dataset
  const sequence = req.params.sequence
  const algorithm = req.params.algorithm

  const basePath = path.join(dataset, sequence)
  const evalPath = path.join(basePath, 'eval', algorithm)
  const imagesLength = utils.countFiles(path.join(basePath, 'left'))

  csv.read(path.join(evalPath, 'aggregated_results.csv'), results => {
    csv.read(path.join(evalPath, 'aggregated_runtime.csv'), runtime => {
      const data = {
        images: imagesLength,
        algorithmName: utils.getAlgorithmName(algorithm),
        algorithm: algorithm,
        selection: dataset + ' / ' + sequence,
        basePath: basePath,
        evalPath: evalPath,
        results: JSON.stringify(_.merge(results, runtime))
      }
      return res.render('result_detail', data)
    })
  })
}
