var monitor = require('active-window');

callback = function(window){
  try {
    console.log("App: " + window.app);
    console.log("Title: " + window.title);
  }catch(err) {
      console.log(err);
  }
}

monitor.getActiveWindow(callback);
