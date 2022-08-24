function init(window, core) {
  core.refreshDownloadProgress.connect(function(value) {
    console.log('received value ' + value);
    window.downloading.value = value / 100;
  });

  window.download.onClicked.connect(core.downloadFile);
}

function launch(window, core) {

}
