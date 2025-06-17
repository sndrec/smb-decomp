// Basic three.js scene that steps the libmkb simulation.
// libmkb.js and libmkb.wasm must be built with Emscripten.

let Module = Module || {};

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

Module.onRuntimeInitialized = () => {
  lib = Module;
  ballPtr = lib._malloc(BALL_SIZE);
  cameraPtr = lib._malloc(CAMERA_SIZE);

  lib._load_stage_collision(1);
  lib._world_sim_init();
  lib._stobj_sim_init();
  lib._item_sim_init();
  lib._stage_anim_init();
  lib._ball_sim_init(ballPtr);
  lib._camera_sim_init(cameraPtr, ballPtr);

  initThree();
  animate();
};
