
var m_someVar = false;

var m_modelsData = {};

function initialize() {
    console.log('m_someVar: %s', m_someVar);
}

function registerModel(modelName, modelDataObj) {
    m_modelsData[modelName] = modelDataObj;
}

function getModelData(modelName) {
    return m_modelsData[modelName];
}

function update() {
    m_someVar = !m_someVar;
    console.log('m_someVar: ' + m_someVar);
}
