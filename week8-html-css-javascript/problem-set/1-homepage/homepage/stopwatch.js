function startStopwatch(h, m, s) {
  let sm = increment(s, m);
  s = sm.minor;
  m = sm.major;

  let mh = increment(m, h);
  m = mh.minor;
  h = mh.major;

  h = formatNumber(h);
  m = formatNumber(m);
  s = formatNumber(s);

  if (h !== "00") {
    document.getElementById("h").innerHTML = h;
    document.getElementById("hm").innerHTML = " : ";
  }

  if (m !== "00" || h !== "00") {
    document.getElementById("m").innerHTML = m;
    document.getElementById("ms").innerHTML = " : ";
  }

  document.getElementById("s").innerHTML = s;
  h = parseInt(h);
  m = parseInt(m);
  s = parseInt(s);
  s++;
  setTimeout(function () {
    startStopwatch(h, m, s);
  }, 1000);
}

function formatNumber(i) {
  if (i < 10) {
    i = "0" + i;
  } // add zero in front of numbers < 10
  return i;
}

function increment(minor, major) {
  if (minor === 60) {
    return { minor: 0, major: ++major };
  }
  return { minor: minor, major: major };
}
