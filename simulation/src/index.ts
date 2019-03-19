import * as Dotenv from 'dotenv';
import * as Koa from 'koa';
import * as Router from 'koa-router';
import * as QueueService from './queue-service';

if (process.env.NODE_ENV !== 'production') {
  Dotenv.config();
}

const app = new Koa();
const router = new Router();

router.get('/', async (ctx) => {
  ctx.body = 'I am simulation service';
});

app.use(router.routes());

app.listen(3000);

QueueService.startConsuming();
