// Basic three.js scene that steps the libmkb simulation.
// libmkb.js and libmkb.wasm must be built with Emscripten.

// Reuse the Module object created by Emscripten if available
var Module = typeof Module !== 'undefined' ? Module : {};

// Keep track of WASD key state for simple tilt control
const keys = { w: false, a: false, s: false, d: false };
function handleKey(e, down) {
  switch (e.key.toLowerCase()) {
    case 'w': keys.w = down; break;
    case 'a': keys.a = down; break;
    case 's': keys.s = down; break;
    case 'd': keys.d = down; break;
  }
}
window.addEventListener('keydown', e => handleKey(e, true));
window.addEventListener('keyup', e => handleKey(e, false));

// Write the current key state to controllerInfo so libmkb can read it
function updateController() {
  if (!lib || lib._controllerInfo === undefined) return;
  const base = lib._controllerInfo; // controllerInfo[0]
  let stickX = 0;
  let stickY = 0;
  if (keys.a) stickX -= 60;
  if (keys.d) stickX += 60;
  if (keys.w) stickY += 60;
  if (keys.s) stickY -= 60;
  lib.HEAP8[base + 2] = stickX; // held.stickX
  lib.HEAP8[base + 3] = stickY; // held.stickY
}

let scene, camera, renderer, ballMesh;
let lib, ballPtr, cameraPtr;
const BALL_SIZE = 0x1A4;   // sizeof(struct Ball)
const CAMERA_SIZE = 0x284; // sizeof(struct Camera)

function initThree() {
  scene = new THREE.Scene();
  const light = new THREE.DirectionalLight(0xffffff, 1);
  light.position.set(5, 10, 7.5);
  scene.add(light);

  const geometry = new THREE.SphereGeometry(1, 32, 32);
  const material = new THREE.MeshStandardMaterial({ color: 0xffc000 });
  ballMesh = new THREE.Mesh(geometry, material);
  scene.add(ballMesh);

  camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
  camera.position.z = 5;

  renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setSize(window.innerWidth, window.innerHeight);
  document.body.appendChild(renderer.domElement);
}

function animate() {
  requestAnimationFrame(animate);

  if (lib) {
    updateController();
    lib._world_sim_step();
    lib._stobj_sim_step();
    lib._item_sim_step();
    lib._stage_anim_step();
    lib._ball_sim_step(ballPtr);
    lib._camera_sim_step(cameraPtr, ballPtr);

    const x = lib.getValue(ballPtr + 0x04, 'float');
    const y = lib.getValue(ballPtr + 0x08, 'float');
    const z = lib.getValue(ballPtr + 0x0C, 'float');
    ballMesh.position.set(x, y, z);
  }

  renderer.render(scene, camera);
}

async function onModuleLoaded() {
  lib = Module;
  const malloc = lib._malloc || (lib.cwrap && lib.cwrap('malloc', 'number', ['number']));
  if (!malloc) {
    console.error('malloc not available in Module');
    return;
  }
  ballPtr = malloc(BALL_SIZE);
  cameraPtr = malloc(CAMERA_SIZE);

  const resp = await fetch('STAGE002.stagedef');
  const buf = new Uint8Array(await resp.arrayBuffer());
  lib.FS_createDataFile('/', 'STAGE002.stagedef', buf, true, true);

  lib._load_stage_collision(2);
  lib._world_sim_init();
  lib._stobj_sim_init();
  lib._item_sim_init();
  lib._stage_anim_init();
  lib._ball_sim_init(ballPtr);
  lib._camera_sim_init(cameraPtr, ballPtr);

  initThree();
  animate();
}

// When using Emscripten with MODULARIZE=1, libmkb.js exports a factory
// function instead of a ready-to-use Module object. Detect that case and
// instantiate the module so onModuleLoaded runs once the runtime is ready.
if (typeof Module === 'function') {
  Module().then(m => { Module = m; onModuleLoaded(); });
} else {
  Module.onRuntimeInitialized = onModuleLoaded;
}
