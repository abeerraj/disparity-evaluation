module.exports.route = function(app, controllers) {
  app.get('/', controllers.index)
  app.get('/list/:dataset/:sequence', controllers.list)
  app.get('/result/:dataset/:sequence/:algorithm', controllers.result)
}
