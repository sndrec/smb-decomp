const {execSync, spawnSync} = require('child_process');
const fs = require('fs');

// Skip the test if emcc is not available
const which = spawnSync('which', ['emcc']);
if (which.status !== 0) {
  console.log('emcc not found, skipping build_wasm test.');
  process.exit(0);
}

try {
  execSync('tools/build_wasm.sh', { stdio: 'inherit' });
} catch (err) {
  console.error('build_wasm.sh failed:', err);
  process.exit(1);
}

if (!fs.existsSync('webdemo/libmkb.wasm')) {
  console.error('libmkb.wasm not produced');
  process.exit(1);
}
console.log('build_wasm.sh completed successfully.');
