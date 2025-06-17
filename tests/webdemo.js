const fs = require('fs');
const vm = require('vm');

// Minimal mocks for browser globals used in main.js
const mockWindow = { addEventListener: () => {} };
const mockDocument = { body: { appendChild: () => {} } };

const mockTHREE = {
  Scene: function () { this.add = () => {}; },
  DirectionalLight: function () { this.position = { set: () => {} }; },
  SphereGeometry: function () {},
  MeshStandardMaterial: function () {},
  Mesh: function () { this.position = { set: () => {} }; },
  PerspectiveCamera: function () { this.position = {}; },
  WebGLRenderer: function () { this.setSize = () => {}; this.domElement = {}; this.render = () => {}; },
};

// Dummy WebAssembly Module with stubs used in onModuleLoaded/animate
const mockModule = {
  HEAP8: new Uint8Array(1024),
  _controllerInfo: 0,
  _malloc: () => 0,
  FS_createDataFile: () => {},
  getValue: () => 0,
  _load_stage_collision: () => {},
  _world_sim_init: () => {},
  _stobj_sim_init: () => {},
  _item_sim_init: () => {},
  _stage_anim_init: () => {},
  _ball_sim_init: () => {},
  _camera_sim_init: () => {},
  _world_sim_step: () => {},
  _stobj_sim_step: () => {},
  _item_sim_step: () => {},
  _stage_anim_step: () => {},
  _ball_sim_step: () => {},
  _camera_sim_step: () => {},
};

const context = vm.createContext({
  window: mockWindow,
  document: mockDocument,
  THREE: mockTHREE,
  Module: mockModule,
  fetch: async () => ({ arrayBuffer: async () => new ArrayBuffer(0) }),
  requestAnimationFrame: () => {},
});

try {
  const src = fs.readFileSync('webdemo/main.js', 'utf8');
  vm.runInContext(src, context, { filename: 'main.js' });
  if (typeof context.Module.onRuntimeInitialized === 'function') {
    context.Module.onRuntimeInitialized();
  }
  console.log('main.js executed in test context.');
} catch (err) {
  console.error('Execution failed:', err);
  process.exit(1);
}
