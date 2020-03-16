function getProbe(probeId) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById(probeId).innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "get"+probeId+"values", true);
  xhttp.send();
}
