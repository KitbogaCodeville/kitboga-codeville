'use strict';
var AWS = require("aws-sdk");

const docClient = new AWS.DynamoDB.DocumentClient();
const ddbTable = process.env.REMOTE_CONTROL_DDBTABLE;

module.exports.handler = (event, context) => {
  console.log(JSON.stringify(event));

  let alertContent = event.Records[0].Sns;
  let message = JSON.parse(alertContent.Message);
  console.log(message);

  let params = {
    TableName: ddbTable,
    Key: {
      'clientID': message.clientID,
      'expireTime': message.expireTime,
    }
  }

  docClient.delete(params, (err, data) => {
    if (err) console.error(err, err.stack);
    else console.log(data);

  })
};
