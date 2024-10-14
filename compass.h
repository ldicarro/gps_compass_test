var headingText;
var arrow;
var tbl;
var wrapper;
var loading;

const update = () => {
  fetch('/data')
    .then(res => res.json())
    .then(json => {

      headingText.textContent = `${(json.hdg * 30)}${String.fromCodePoint(0x00B0)} (${json.hdg})`;
      arrow.style.transform = `rotate(${(json.hdg * 30) - 90}deg)`;

      const keys = Object.keys(json);

      tbl.innerHTML = '';

      keys.forEach(key => {
        if(key === 'hdg') {
          return;
        }
        const row = document.createElement('tr');
        const td1 = document.createElement('td');
        const td2 = document.createElement('td');

        td1.textContent = key.charAt(0).toUpperCase() + key.slice(1);
        td2.textContent = json[key];

        row.appendChild(td1);
        row.appendChild(td2);

        tbl.appendChild(row);
      });

      wrapper.classList.remove('hidden');
      loading.classList.add('hidden');
  })
}

window.addEventListener('DOMContentLoaded', () => {
  headingText = document.querySelector('#heading-value');
  arrow = document.querySelector('#arrow');
  tbl = document.querySelector('#tbl');
  wrapper = document.querySelector('.wrapper');
  loading = document.querySelector('.loading');

  const t = setInterval(update, 1000);
});