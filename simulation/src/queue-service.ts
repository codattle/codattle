import * as amqp from 'amqplib/callback_api';

const QUEUE_NAME = 'simulate';

export let startConsuming = () => {
  amqp.connect(process.env.QUEUE_URL as string, (_error, connection) => {
    connection.createChannel((_error, channel) => {
      channel.assertQueue(QUEUE_NAME, { durable: false });
      channel.consume(QUEUE_NAME, (message) => {
        if (message) {
          console.log('Received %s', message.content.toString());
          channel.ack(message);
        }
      });
    });
  });
};
