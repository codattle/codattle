import * as Koa from 'koa';
import * as Router from 'koa-router';
import * as amqp from 'amqplib/callback_api'

const QUEUE_NAME = "simulate"

const app = new Koa();
const router = new Router();

router.get('/', async (ctx) => {
    ctx.body = 'I am simulation service';
});

app.use(router.routes());

app.listen(3000);

amqp.connect('amqp://localhost', (error, connection) => {
    connection.createChannel((error, channel) => {
        channel.assertQueue(QUEUE_NAME, {durable: false})
        channel.consume(QUEUE_NAME, message => {
            if (message) {
                console.log("Received %s", message.content.toString());
            }  
        })
    })
})
