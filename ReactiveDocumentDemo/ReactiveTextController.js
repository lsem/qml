.import 'Utils.js' as Utils


var internals = {
    m_dragStartMouseX: null,
    m_draggedModelVar: null,
    m_modelInitialValue: null,
    m_dataModelRef: null,
    m_containerMouseAreRe: null,
    m_toolTip: false,

    createHoverToolTipElement: function(parentObject) {
        Utils.createComponent('HoverToolTipComponent.qml',
            parentObject,
            function(instance) {
                internals.m_toolTip = instance;
            }, function(err) {
                console.log('Failed to crate hover tool object: ' + err);
            });
    },
    isModelDraggable: function (modelVar) {
        return modelVar === 'appleCount'; // just for demo, we can ask m_dataModelRef is it draggable or not
    }
};

function initialize(dataModel, containerMouseArea, toolTipParent) {
    internals.m_dataModelRef = dataModel;
    internals.m_containerMouseAreRef = containerMouseArea;

    internals.createHoverToolTipElement(toolTipParent);
}

function onReactiveNumberHovered(x, y, modelVar) {
    if (!internals.isModelDraggable(modelVar) && !internals.m_draggedModelVar) {
        internals.m_toolTip.visible = false;
        return;
    }

    if (internals.m_draggedModelVar) {
        var min = internals.m_dataModelRef[internals.m_draggedModelVar + '_Minimum'];
        var max = internals.m_dataModelRef[internals.m_draggedModelVar + '_Maximum'];
        var step = internals.m_dataModelRef[internals.m_draggedModelVar + '_Step'] || 1;
        var deltaXPerStep = 3;
        var dragZozeWidth = 100;

        var xdiff = x - internals.m_dragStartMouseX;
        var stepsTaken = Math.floor(xdiff / deltaXPerStep);

        var newValue = internals.m_modelInitialValue + (stepsTaken * step);

        if ((typeof max !== 'undefined') && newValue > max)
            newValue = max;
        if ((typeof min !== 'undefined') && newValue < min)
            newValue = min;

        internals.m_dataModelRef[internals.m_draggedModelVar] = newValue;
        internals.m_toolTip.visible = false;
    } else if (modelVar) {
        if (!internals.m_toolTip.visible) {
            internals.m_toolTip.x = x - 10;
            internals.m_toolTip.y = y - 30;
            internals.m_toolTip.width = 30;
            internals.m_toolTip.height= 30;
            internals.m_toolTip.visible = true;
        }
    } else {
        internals.m_toolTip.visible = false;
    }
}

function onReactiveNumberPressed(x, y, modelVar) {
    if (!internals.isModelDraggable(modelVar))
        return;

    internals.m_draggedModelVar = modelVar
    internals.m_dragStartMouseX = x;
    internals.m_modelInitialValue = internals.m_dataModelRef[modelVar];
    internals.m_containerMouseAreRef.cursorShape = Qt.SplitHCursor;
    internals.m_toolTip.visible = false;
}

function onReactiveNumberRleased(x, y, modelVar) {
    internals.m_draggedModelVar = null;
    internals.m_dragStartMouseX = null;
    internals.m_modelInitialValue = null;
    internals.m_containerMouseAreRef.cursorShape = Qt.ArrowCursor;

    onReactiveNumberHovered(x, y, modelVar);
}

function onReactiveTextMouseLeaved(x, y, modelVar) {
    internals.m_toolTip.visible = false;
}
