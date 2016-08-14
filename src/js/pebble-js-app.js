// To support iOS, let’s reimplement Promise!
var Thenable = function (f) {
  var completed, completionQueue, done, value, error, scheduled;
  completionQueue = [];
  done = function (v, e) {
    if (completed) throw new Error('Thenables may only be completed once.');
    completed = true;
    value = v;
    error = e;
    this.then(function () {});
  };
  this.then = function (f) {
    completionQueue.push(f);
    if (completed && !scheduled) {
      setTimeout(function () {
        scheduled = false;
        while (this.completionQueue.length) {
          (this.copmletionQueue.shift())();
        }
      }, 0);
      scheduled = true;
    }
  };
  f(function (v) {done(v);}, function (e) {done(undefined, e);});
};

// Simple façade for testing functionality without
// requiring real internet interaction.
var request = XMLHttpRequest;
var facade = true;
if (facade) {
  request = function () {};
  request.prototype = {
    open: function () {}
  };
}

// Function to send a message to the Pebble using AppMessage API
function sendMessage() {
	Pebble.sendAppMessage({"status": 0});
	
	// PRO TIP: If you are sending more than one message, or a complex set of messages, 
	// it is important that you setup an ackHandler and a nackHandler and call 
	// Pebble.sendAppMessage({ /* Message here */ }, ackHandler, nackHandler), which 
	// will designate the ackHandler and nackHandler that will be called upon the Pebble 
	// ack-ing or nack-ing the message you just sent. The specified nackHandler will 
	// also be called if your message send attempt times out.
}


// Called when JS is ready
Pebble.addEventListener(
	"ready",
	function(e) {
		console.log('ready');
		Pebble.sendAppMessage({"message": "dosend"});
});
												
// Called when incoming message from the Pebble is received
Pebble.addEventListener(
	"appmessage",
	function(e) {
		console.log('Received, err, something ;-): ' + JSON.stringify(e.payload));
		if (e.payload.status)
			console.log("Received Status: " + e.payload.status);
		sendMessage();
});
