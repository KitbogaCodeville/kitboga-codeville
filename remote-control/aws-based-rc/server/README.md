# Serverless Remote Control API

## Requirements

- [Serverless](https://serverless.com/framework/docs/providers/aws/guide/quick-start/) installed and configured (including the [AWS CLI](https://docs.aws.amazon.com/cli/latest/userguide/installing.html))
- Environment variables set for:
  - `REMOTE_CONTROL_API_SECRET_KEY` = Any string that will be used for validation of GET/POST requests (used as `?key=REMOTE_CONTROL_API_SECRET_KEY`)

## API Reference

The current API just passes through the `action` and `data` values of the JSON POST verbatim with no validation. The `clientID` is provided at the end of the path, and the secret key is provided as a querystring.

### Sample Data:

```js
POST TO: APIURL/clientID?key=APIKEY
{
    action: "someaction",
    data: {
        "something": "Value",
        "stuff": "things"
    }
}
```

## Limitations

- Very limited UI available on the root node, needs to be a better SPA
- Some error handling is missing
- Client app (for controlled PC) is not currently included

## Support

This is provided with no support whatsoever, as it is a personal project. If you use it for something, I'd love to hear from you.

## Credits

Currently all code written by myself with no contributions.