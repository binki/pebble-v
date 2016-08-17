'use strict';

var Promise = require('bluebird');
var url = require('url');

var endpoint = 'http://brinkdatabase.com/sii';
var pinUri = endpoint + '/scripts/pin-gen.php';
var dataUri = endpoint + '/scripts/data-gen.php';

localStorage.setItem();

var always = function (thenable, action) {
  return thenable.then(function (value) {
    action();
    return value;
  }, function (ex) {
    action();
    return Promise.reject(ex);
  });
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

var AjaxError = function (xhr, status, text) {
  this.xhr = xhr;
  this.status = status;
  this.text = text;
}
AjaxError.prototype = new Error();

/**
 * See authenticatedAjax().
 */
var ajax = function (options) {
  return new Promise(function (resolve, reject) {
    options.method = options.method || 'GET';
    if (!options.uri) {
      throw new Error('options.uri must be specified');
    }

    var xhr = new XMLHttpRequest();
    xhr.addEventListener('load', function () {
      try
      {
        console.log('response: ' + this.responseText);
        resolve(JSON.parse(this.responseText));
      }
      catch (ex)
      {
        reject(ex);
      }
    });
    xhr.addEventListener('error', function () {
      if (this.status === 0
        || this.status >= 500 && this.status < 600) {
        // Retry
        console.log('Got response: ' + this.status + '. Retrying in 8.');
        setTimeout(function () {
          ajax.then(resolve, reject);
        }, 8000);
      } else {
        reject(new AjaxError(xhr, this.status, this.statusText));
      }
    });
    console.log('opening ' + options.uri);
    xhr.open(options.method, options.uri);
  });
};

/**
 * \param options
 *   With keys:
 *     uri
 *     data
 * \returns
 *   Promise
 */
var authenticatedAjax = function (options) {
  return getAccessToken().then(function (access_token) {
    console.log('got access token!');
    var parsedUri = url.parse(options.uri, true);
    parsedUri.query.access_token = access_token;
    options.uri = url.format(parsedUri);
    return ajax(options).catch(function (ex) {
      if (ex instanceof AjaxError) {
        if (ex.status == 403) {
          // The API has rejected our token. So mark our token as dead and continue.
          localStorage.removeItem('access_token');
          // Try again.
          return authenticatedAjax(options);
        }
      }
      return Promise.reject(ex);
    });
  });
};

var gettingAccessToken = null;
/**
 * Get the access token or start the workflow to get one.
 */
var getAccessToken = function () {
  if (!gettingAccessToken) {
    gettingAccessToken = always(new Promise(function (resolve, reject) {
      var access_token = localStorage.getItem('access_token');
      console.log('access token found in storage: ' + access_token);
      if (access_token) {
        return resolve(access_token);
      }
      // Start a PIN account binding.
      resolve(ajax({
        uri: pinUri,
      }).then(function (data) {
        throw new Error('I still don’t know how to get an access token—even though I got the PIN: ' + JSON.stringify(data));
      }));
    }), function () {
      gettingAccessToken = null;
    });
  }
  return gettingAccessToken;
};

var currentUpload = null;
var upload = function () {
  if (currentUpload) {
    return currentUpload;
  }
  return currentUpload = always(authenticatedAjax({
    uri: 'asdf',
  }), function () {
    currentUpload = null;
  });
};

var currentDownload = null;
var download = function () {
  if (currentDownload) {
    return currentDownload;
  }
  return currentDownload = always(authenticatedAjax({
    uri: dataUri,
  }).then(function (data) {
    console.log('Got data! ' + JSON.stringify(data));
  }), function () {
    currentDownload = null;
  });
};

// Called when JS is ready
Pebble.addEventListener(
	"ready",
	function(e) {
		console.log('ready');
    // Let watch know it can start uploading data to us if it wants.
		Pebble.sendAppMessage({MESSAGE: 'READY'});
    // Upload any logs.
    upload().catch(function (ex) {console.log('threw');console.log(ex);});
    // Download updated data.
    download().catch(function (ex) {console.log('here');console.log(ex);});
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
