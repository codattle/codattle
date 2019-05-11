import { Schema } from 'mongoose';

export interface IResultFrame {
  order: number;
  content: Map<string, object>;
}

export const ResultFrameSchame: Schema = new Schema({
  order: { type: Number, required: true },
  content: { type: Map, required: true },
});
