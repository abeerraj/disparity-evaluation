const ECT = require('ect')

module.exports.bind = function(app) {
  const ectRenderer = ECT({ watch: true, root: __dirname + '/../../views', ext : '.ect' })
  app.set('view engine', 'ect')
  app.engine('ect', ectRenderer.render)
}
