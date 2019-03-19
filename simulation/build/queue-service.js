"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const amqp = require("amqplib/callback_api");
const QUEUE_NAME = 'simulate';
exports.startConsuming = () => {
    amqp.connect(process.env.QUEUE_URL, (_error, connection) => {
        connection.createChannel((_error, channel) => {
            channel.assertQueue(QUEUE_NAME, { durable: false });
            channel.consume(QUEUE_NAME, (message) => {
                if (message) {
                    console.log('Received %s', message.content.toString());
                }
            });
        });
    });
};
