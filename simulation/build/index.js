"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Koa = require("koa");
const Router = require("koa-router");
const QueueService = require("./queue-service");
const app = new Koa();
const router = new Router();
router.get('/', async (ctx) => {
    ctx.body = 'I am simulation service';
});
app.use(router.routes());
app.listen(3000);
QueueService.startConsuming();
