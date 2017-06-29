const life = (function() {
  const GRID_WIDTH = 400;
  const GRID_HEIGHT = 400;

  let state;

  let gfx;
  let cellSize = 10;
  let baseX = -10;
  let baseY = -10;
  
  let dragStartX, dragStartY, dragging;
  let origBaseX, origBaseY;

  $(document).ready(function() {
    gfx = $("#grid")[0].getContext("2d");
    gfx.canvas.width = GRID_WIDTH;
    gfx.canvas.height = GRID_HEIGHT;
  });

  function drawGridLines() {
    gfx.lineWidth = 1;
    gfx.strokeStyle = "#eee";
    gfx.beginPath();
    for (let x = cellSize; x < GRID_WIDTH; x += cellSize) {
      gfx.moveTo(x, 0);
      gfx.lineTo(x, GRID_HEIGHT);
    }
    for (let y = GRID_HEIGHT - cellSize; y > 0; y -= cellSize) {
      gfx.moveTo(0, y);
      gfx.lineTo(GRID_WIDTH, y);
    }
    gfx.stroke();
  }

  function drawCells(cells) {
    const maxX = baseX + GRID_WIDTH / cellSize;
    const maxY = baseY + GRID_HEIGHT / cellSize;
    gfx.fillStyle = "#000";
    for (let i = 0; i < cells.size(); i++) {
      const [x, y] = cells.get(i);
      if (x < baseX || x > maxX || y < baseY || y > maxY) {
        continue;
      }
      const bx = (x - baseX) * cellSize;
      const by = GRID_HEIGHT - (y - baseY + 1) * cellSize;
      gfx.fillRect(bx + 1, by + 1, cellSize - 2, cellSize - 2);
    }
  }

  function drawGrid() {
    gfx.fillStyle = "#fff";
    gfx.fillRect(0, 0, gfx.canvas.width, gfx.canvas.height);

    drawGridLines();
    const cells = state.getAlive();
    drawCells(cells);
    cells.delete();
  }

  return {
    onTemplateChanged: function() {
      const templates = {
        "": "",
        "Glider": "(0, 2)\n(1, 1)\n(2, 1)\n(0, 0)\n(1, 0)",
        "Static Box": "(0, 0)\n(1, 0)\n(0, 1)\n(1, 1)",
        "Blinker": "(1, 2)\n(1, 1)\n(1, 0)",
        "Diehard": "(6, 2)\n(0, 1)\n(1, 1)\n(1, 0)\n(5, 0)\n(6, 0)\n(7, 0)",
      };

      const tmpl = $("#template-in option:selected").text();
      $("#cells-in").val(templates[tmpl]);
    },

    onStartClicked: function() {
      const cells = Module.parseCellList($("#cells-in").val());
      state = new Module.Life(cells);
      state.step();
      drawGrid();

      const fadeMs = 150;
      $("#start-panel").fadeOut(fadeMs, function() {
        $("#life-panel").fadeIn(fadeMs);
      });
    },

    gridMouseDown: function(evt) {
      dragStartX = evt.clientX;
      dragStartY = evt.clientY;
      dragging = true;
      origBaseX = baseX;
      origBaseY = baseY;
    },

    gridMouseMove: function(evt) {
      if (!dragging) {
        return;
      }
      const dx = Math.floor((evt.clientX - dragStartX) / 10);
      const dy = Math.floor((evt.clientY - dragStartY) / 10);
      baseX = origBaseX - dx;
      baseY = origBaseY + dy;
      $("#base-x").val(baseX);
      $("#base-y").val(baseY);
      drawGrid();
    },

    gridMouseUp: function(evt) {
      dragging = false;
    },

    onBaseChanged: function() {
      baseX = parseInt($("#base-x").val());
      baseY = parseInt($("#base-y").val());
      drawGrid();
    },

    onZoomChanged: function() {
      cellSize = parseInt($("#zoom").val()) * 5;
      drawGrid();
    },

    onStepClicked: function() {
      state.step();
      drawGrid();
    },
  };
}());
