var config = require('./lib/app/config')
var renderer = require('./lib/app/renderer')
var routes = require('./lib/app/routes')
var utils = require('./lib/common/utils')
var express = require('express')
var bodyParser = require('body-parser')

var app = express()

app.use(bodyParser.json())
app.use(bodyParser.urlencoded({ extended: true }))

app.use('/public', express.static(__dirname + '/public'))
app.use('/static', express.static(config.datasets))

renderer.bind(app)
routes.route(app, utils.controllers())

app.listen(config.port, function() {
  console.log('app is listening on port ' + config.port)
})
