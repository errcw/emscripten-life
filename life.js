const life = (function() {
  let state;

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
      // TODO(eworoshow): Use the cells.
      const cells = Module.parseCellList($("#cells-in").val());
      console.log(cells.size());
      state = new Module.Life();

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
