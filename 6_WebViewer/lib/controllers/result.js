const utils = require('../common/utils')
const path = require('path')

module.exports = function(req, res) {
  const dataset = req.params.dataset
  const sequence = req.params.sequence
  const algorithm = req.params.algorithm

  const basePath = path.join(dataset, sequence)
  const evalPath = path.join(basePath, 'eval', algorithm)

  const imagesLength = utils.countFiles(path.join(basePath, 'left'))

  const data = {
    images: imagesLength,
    algorithmName: utils.getAlgorithmName(algorithm),
    algorithm: algorithm,
    selection: dataset + ' / ' + sequence,
    basePath: basePath,
    evalPath: evalPath
  }

  return res.render('result_detail', data)
}
