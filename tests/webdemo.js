const fs = require('fs');
const vm = require('vm');

// Minimal mocks for browser globals used in main.js
const mockWindow = {
  addEventListener: () => {},
};

const context = vm.createContext({
  window: mockWindow,
  document: {},
  Module: {}
});

try {
  const src = fs.readFileSync('webdemo/main.js', 'utf8');
  vm.runInContext(src, context, { filename: 'main.js' });
  console.log('main.js executed in test context.');
} catch (err) {
  console.error('Execution failed:', err);
  process.exit(1);
}
