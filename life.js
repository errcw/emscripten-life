function interact() {
  var t = new Module.Life();
  var v = t.get();
  var v0 = v.get(0);
  t.delete();

  var vv = Module.parseCellList("(1, 1)");
  var vv0 = vv.get(0);
}
