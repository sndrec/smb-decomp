const fs = require('fs');
const path = require('path');

(async () => {
  const createMKBModule = require('../webdemo/libmkb.js');
  let Module;
  try {
    // Node's fetch does not handle file:// URLs reliably; disable streaming
    global.fetch = undefined;
    Module = await createMKBModule({
      locateFile: (file) => path.resolve(__dirname, '../webdemo', file)
    });
  } catch (err) {
    console.error('Failed to initialise WebAssembly module:', err.message);
    process.exit(1);
  }

  const BALL_SIZE = 0x1A4;
  const CAMERA_SIZE = 0x284;
  const ballPtr = Module._malloc(BALL_SIZE);
  const cameraPtr = Module._malloc(CAMERA_SIZE);

  // Load stage data if available
  try {
    const buf = fs.readFileSync(path.join(__dirname, '../webdemo/STAGE002.stagedef'));
    Module.FS_createDataFile('/', 'STAGE002.stagedef', buf, true, true);
    Module._load_stage_collision(2);
  } catch (err) {
    console.error('Failed to load stage data:', err.message);
    process.exit(1);
  }

  Module._world_sim_init();
  Module._stobj_sim_init();
  Module._item_sim_init();
  Module._stage_anim_init();
  Module._ball_sim_init(ballPtr);
  Module._camera_sim_init(cameraPtr, ballPtr);

  // Advance the simulation several frames before taking measurements
  for (let i = 0; i < 500; i++) {
    Module._world_sim_step();
    Module._stobj_sim_step();
    Module._item_sim_step();
    Module._stage_anim_step();
    Module._ball_sim_step(ballPtr);
    Module._camera_sim_step(cameraPtr, ballPtr);
  }

  function getBall() {
    return [
      Module.getValue(ballPtr + 0x04, 'float'),
      Module.getValue(ballPtr + 0x08, 'float'),
      Module.getValue(ballPtr + 0x0C, 'float'),
    ];
  }

  function getCam() {
    return [
      Module.getValue(cameraPtr + 0x00, 'float'),
      Module.getValue(cameraPtr + 0x04, 'float'),
      Module.getValue(cameraPtr + 0x08, 'float'),
    ];
  }

  const initialBall = getBall();
  const initialCam = getCam();

  for (let i = 0; i < 5; i++) {
    if (typeof Module._controllerInfo !== 'undefined') {
      const base = Module._controllerInfo;
      Module.setValue(base + 2, Math.floor(Math.random() * 121) - 60, 'i8');
      Module.setValue(base + 3, Math.floor(Math.random() * 121) - 60, 'i8');
    }
    Module._world_sim_step();
    Module._stobj_sim_step();
    Module._item_sim_step();
    Module._stage_anim_step();
    Module._ball_sim_step(ballPtr);
    Module._camera_sim_step(cameraPtr, ballPtr);
  }

  const finalBall = getBall();
  const finalCam = getCam();

  const ballMoved = initialBall.some((v, i) => v !== finalBall[i]);
  const camMoved = initialCam.some((v, i) => v !== finalCam[i]);

  if (!ballMoved && !camMoved) {
    console.error('Simulation did not update positions.');
    process.exit(1);
  }
  console.log('Simulation updated positions successfully.');
})();
