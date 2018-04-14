'use strict';
var AWS = require("aws-sdk");

var ddbClient = new AWS.DynamoDB();
const ddbTable = process.env.REMOTE_CONTROL_DDBTABLE;

function ddbInsert(clientid, expiry, action, data, callback) {
  console.log('Table Item Expiry Stamp: '+expiry);
  let params = {
    TableName: ddbTable,
    Item: {
      clientID: { S: clientid },
      expireTime: { N: expiry },
      action: { S: action },
      data: { S: data },
    },
  };
  ddbClient.putItem(params, callback);
}

module.exports.handler = (event, context, callback) => {
  console.log(JSON.stringify(event));

  let apiKey = null
  if ((event.queryStringParameters !== null) &&
  ('key' in event.queryStringParameters)) {
    apiKey = event.queryStringParameters['key'];
  }

  if (process.env.REMOTE_CONTROL_API_SECRET_KEY == apiKey) {
    let clientid = event.pathParameters.clientid;
    let bodydata = JSON.parse(event.body);
    let action = bodydata.action;
    let data = JSON.stringify(bodydata.data);
    let expiretime = '' + (Math.floor(Date.now() / 1000) + (60 * 60 * 2)) // Last digit is number of hours to persist the entry

    ddbInsert(clientid, expiretime, action, data, (err, resp) => {
      if (err) console.error(err, err.stack);
      else console.log(resp);
      const response = {
        statusCode: 200,
        body: JSON.stringify({
          success: "true",
          clientID: clientid,
          expireTime: expiretime,
          action: action,
          data: data,
        }),
      }
      callback(null, response);
    });

  } else {
    console.error('API Key Mismatch: '+apiKey);
    const response = {
      statusCode: 402, // HTTP 402: Payment Required
      body: JSON.stringify({
        message: 'FIXATION ON HOLD: Please insert more Googul Pley cards to know each and everything.',
      }),
    }
    callback(null, response);
  }
};
