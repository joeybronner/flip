var skypeAPI = require('skype-api');

var skype = skypeAPI();

skype.getUser('joey.bronner', function (err, user) {
    console.log(user);
});