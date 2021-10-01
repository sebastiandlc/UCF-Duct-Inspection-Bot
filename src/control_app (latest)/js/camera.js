var video = document.getElementById('videoElement');
var canvas;
var context;

navigator.mediaDevices.getUserMedia({video: true}) // request cam
.then(stream => {
  video.srcObject = stream; // don't use createObjectURL(MediaStream)
  return video.play(); // returns a Promise
})
.then(()=>{ // enable the button
  var btn = document.getElementById('cameraButton');
  btn.onclick = e => {
    takePicture();
  };
})

function takePicture() {
  canvas = document.getElementById('canvas'); // create a canvas
  context = canvas.getContext('2d'); // get its context
  context.drawImage(video, 0, 0, canvas.width, canvas.height);

  let a = document.createElement('a');
  a.href = canvas.toDataURL('image/jpeg');
  a.download = 'screenshot.jpg';
  //document.body.appendChild(a);
  a.click();

  context.clearRect(0, 0, canvas.width, canvas.height);
}
