import { spawn } from 'child_process';
import { platform } from 'os';

const gyp = 'node-gyp';

// TODO: Find a better way to find <node_api.h>
const cachePath =
  platform() === 'win32' ? `${process.env['LocalAppData'.toUpperCase()]}/${gyp}/Cache` : `~/.cache/${gyp}`;

const includes = [
  'node_modules/node-addon-api', // napi.h
  `${cachePath}/${process.version.slice(1)}/include/node`, // node_api.h
];

const args = [...includes.map(i => `--extra-arg=-I${i}`), ...process.argv.slice(2)];

console.log('> clang-tidy', args);

spawn('clang-tidy', args, { stdio: 'inherit' });
