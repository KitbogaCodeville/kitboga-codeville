'use strict';
var AWS = require("aws-sdk");

const docClient = new AWS.DynamoDB.DocumentClient();
const ddbTable = process.env.REMOTE_CONTROL_DDBTABLE;

const sns = new AWS.SNS();
const snsarn = process.env.REMOTE_CONTROL_DELETE_SNSARN;

module.exports.handler = (event, context, callback) => {
  console.log(JSON.stringify(event));

  let apiKey = null
  if ((event.queryStringParameters !== null) &&
  ('key' in event.queryStringParameters)) {
    apiKey = event.queryStringParameters['key'];
  }

  if (process.env.REMOTE_CONTROL_API_SECRET_KEY == apiKey) {

    let clientid = event.pathParameters.clientid;
    let params = {
      TableName: ddbTable,
      KeyConditionExpression: "clientID = :clientid",
      ExpressionAttributeValues: {
        ":clientid": clientid,
      },
    }

    docClient.query(params, (err, data) => {
      if (err) console.error(err, err.stack)
      else console.log(data)

      let commands = data.Items;
    
      for (let i = 0, len = commands.length; i < len; i++) {
        console.log(JSON.stringify(commands[i]));

        sns.publish({
          Message: JSON.stringify(commands[i]),
          TopicArn: snsarn,
        }, (err, data) => {
          if (err) console.error('Error pushing to SNS: '+err);
          else console.log(data);
        });
      }

      const response = {
        statusCode: 200,
        body: JSON.stringify(commands),
      };
      callback(null, response);
    })

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
