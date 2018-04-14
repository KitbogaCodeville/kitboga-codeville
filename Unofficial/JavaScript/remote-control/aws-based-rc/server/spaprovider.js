'use strict';

const spaHTML = `<body onload="rebuild()">
<head>
    <title>Remote Control Message Poster</title>
    <script lang="JavaScript">
        var posturl = [location.protocol, '//', location.host, location.pathname].join('');
        var data = "";
        function rebuild(){
            posturl = [location.protocol, '//', location.host, location.pathname, '/'].join('') +
                document.getElementById('clientid').value + 
                '?key=' +
                document.getElementById('apikey').value;
                // + '&nocache=' + (new Date()).getTime();

            data = 
                '{ "action" : "' +
                document.getElementById('action').value +
                '",\\n"data" : {\\n' +
                document.getElementById('data').value +
                '\\n} }';
            
            document.getElementById('json').innerText = data;

        }
        function postdata() {
            var oReq = new XMLHttpRequest();
            // oReq.addEventListener("progress", updateProgress);
            oReq.addEventListener("load", transferComplete);
            oReq.addEventListener("error", transferFailed);
            oReq.addEventListener("abort", transferCanceled);
            oReq.open("POST", posturl);
            oReq.send(data);
        }
        function transferComplete() {
            document.getElementById('output').innerText = this.responseText;
        }
        function transferFailed() {
            document.getElementById('output').innerText = 'POST Failed.';
        }
        function transferCanceled() {
            document.getElementById('output').innerText = 'Cancelled by user.';
        }
    </script>
</head>
<p>
API Key:<br /><input type="inputbox" size="50" id="apikey" onchange="rebuild()" />
</p><p>
Client ID:<br /><input type="inputbox" size="50" id="clientid" onchange="rebuild()" />
</p><p>
Action:<br /><input type="inputbox" size="50" id="action" onchange="rebuild()" />
</p><p>
Data (JSON):<br /><textarea rows="4" cols="50" id="data" onchange="rebuild()"></textarea>
</p><p>
Raw (JSON):<br /><pre id="json"></pre>
</p><p>
<input type="button" onclick="postdata()" value="Post!" />
</p><p>
Output:<br /><pre id="output"></pre>
</p>
</body>`;

module.exports.handler = (event, context, callback) => {
  const response = {
    statusCode: 200,
    headers: {
      'Content-Type': 'text/html'
    },
    body: spaHTML
  };

  callback(null, response);

};
