const life = (function() {
  let state;

  return {
    onTemplateChanged: function() {
      const templates = {
        "": "",
        "Glider": "(0, 2)\n(1, 1)\n(2, 1)\n(0, 0)\n(1, 0)",
        "Static Box": "(0, 0)\n(1, 0)\n(0, 1)\n(1, 1)",
        "Blinker": "(1, 2)\n(1, 1)\n(1, 0)",
      };

      const tmpl = $("#template-in option:selected").text();
      $("#cells-in").val(templates[tmpl]);
    },

    onStartClicked: function() {
      const cells = Module.parseCellList($("#cells-in").val());
      for (let i = 0; i < cells.size(); i++) {
        const cell = cells.get(i);
        console.log("Adding at " + cell);
      }
      state = new Module.Life(cells);
      for (let i = 0; i < 100; i++) {
        state.step();
      }
      const alive = state.getAlive();
      for (let i = 0; i < alive.size(); i++) {
        console.log("Alive at " + alive.get(i));
      }
      alive.delete();
      cells.delete();

      const fadeMs = 150;
      $("#start-panel").fadeOut(fadeMs, function() {
        $("#life-panel").fadeIn(fadeMs);
      });
    },

    gridMouseDown: function(evt) {
      // TODO(eworoshow): Implement.
    },

    gridMouseMove: function(evt) {
      // TODO(eworoshow): Implement.
    },
  };
}());
