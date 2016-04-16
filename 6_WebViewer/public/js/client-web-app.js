var pos = 1
var fps = 200
var isPlaying = false

function bindClickListeners() {
  $('#btn-back').click(function () {
    if (pos == 1) pos = max
    else pos--
    loadNewPosition(pos)
  })
  $('#btn-next').click(function () {
    if (pos == max) pos = 0
    pos++
    loadNewPosition(pos)
  })
  $('#btn-play').click(function () {
    play()
  })
}

function addLeadingZeros(pos) {
  var s = String(pos)
  while(s.length < 4) s = '0' + s
  return s
}

function bindSlider() {
  $('.slider').val(fps)
  $('.slider').jRange({
    from: 50,
    to: 2000,
    step: 50,
    scale: [50,2000],
    width: 300,
    showLabels: true,
    theme: 'theme-blue',
    snap: true,
    onstatechange: function(val) {
      fps = val
    }
  })
}

function initClientWebApp() {
  $('#current').text(pos)
  $('#total').text(max)
  $('#progressbar').width(pos / max * 100 + '%')
  preloadImages()
}

function preloadImages() {
  var images = []
  for (var i = 1; i <= max; i++) {
    var filename = addLeadingZeros(i)
    images.push('/static/' + basePath + '/left/image' + filename + '.png')
    images.push('/static/' + basePath + '/right/image' + filename + '.png')

    images.push('/static/' + basePath + '/masks/image' + filename + '-mask-depth-discontinuity.png')
    images.push('/static/' + basePath + '/masks/image' + filename + '-mask-textured.png')
    images.push('/static/' + basePath + '/masks/image' + filename + '-mask-occluded.png')
    images.push('/static/' + basePath + '/masks/image' + filename + '-mask-salient.png')

    images.push('/static/' + evalPath + '/image' + filename + '-heatmap-disparity.png')
    images.push('/static/' + evalPath + '/image' + filename + '-heatmap-ground-truth.png')
    images.push('/static/' + evalPath + '/image' + filename + '-heatmap-outliers.png')
  }
  $(images).each(function (i, val) {
    $('<img />').attr('src', val).appendTo('body').css('display', 'none')
  })
}

function play() {
  if (isPlaying) {
    $('#btn-play').text('Play')
    isPlaying = false
    return
  }
  $('#btn-play').text('Pause')
  isPlaying = true
  playNext()
}

function playNext() {
  if (!isPlaying) return
  if (pos == max) {
    pos = 0
  }
  pos++
  loadNewPosition(pos)
  setTimeout(playNext, fps)
}

function loadNewPosition(pos) {
  $('#current').text(pos)
  $('#progressbar').width(pos / max * 100 + '%')

  var filename = addLeadingZeros(pos)

  $('#left').attr('src', '/static/' + basePath + '/left/image' + filename + '.png')
  $('#right').attr('src', '/static/' + basePath + '/right/image' + filename + '.png')

  $('#dd').attr('src', '/static/' + basePath + '/masks/image' + filename + '-mask-depth-discontinuity.png')
  $('#tex').attr('src', '/static/' + basePath + '/masks/image' + filename + '-mask-textured.png')
  $('#occ').attr('src', '/static/' + basePath + '/masks/image' + filename + '-mask-occluded.png')
  $('#sal').attr('src', '/static/' + basePath + '/masks/image' + filename + '-mask-salient.png')

  $('#disparity').attr('src', '/static/' + evalPath + '/image' + filename + '-heatmap-disparity.png')
  $('#ground-truth').attr('src', '/static/' + evalPath + '/image' + filename + '-heatmap-ground-truth.png')
  $('#outliers').attr('src', '/static/' + evalPath + '/image' + filename + '-heatmap-outliers.png')
}

$(function() {
  bindSlider()
  bindClickListeners()
  initClientWebApp()
})
