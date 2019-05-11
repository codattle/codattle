import { Schema } from 'mongoose';
import { IResultFrame, ResultFrameSchame } from './result-frame';

export interface IResult {
  winner: number | null;
  resultFrames: IResultFrame[];
}

export const ResultSchema: Schema = new Schema({
  winner: { type: Number, required: false },
  resultFrames: [ResultFrameSchame],
});
