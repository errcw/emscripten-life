const life = (function() {
  const state = new Module.Life();

  return {
    onTemplateChanged: function() {
      const templates = {
        "": "(0, 0)",
        "Glider": "(1, 1)\n(2, 2)",
      };

      const tmpl = $("#template-in option:selected").text();
      $("#cells-in").val(templates[tmpl]);
    },

    onStartClicked: function() {

      const cells = Module.parseCellList($("#cells-in").val());
      for (let i = 0; i < cells.size(); i++) {
        const cell = cells.get(i);
        console.log("Adding at " + cell);
        state.addAt(cell[0], cell[1]);
      }

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
