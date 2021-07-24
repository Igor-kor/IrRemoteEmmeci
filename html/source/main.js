const requestRemote = new XMLHttpRequest();
const requestReload = new XMLHttpRequest();

function getValues() {
    if (requestReload.readyState === 4 && requestReload.status === 200) {
        document.getElementById("temp").innerText = requestReload.response.Temperature;
        document.getElementById("hum").innerText =  requestReload.response.Humidity;
        document.getElementById("Layer_4").style.display = "none";
    }
    if(requestReload.readyState === 4  && requestReload.status !== 200){
        document.getElementById("Layer_4").style.display = "inline-block";
    }
}

requestReload.addEventListener("readystatechange", getValues);
setInterval(() => {
    requestReload.open('GET',  document.URL + 'temp', true);
    requestReload.responseType = 'json';
    requestReload.send(null);
}, 2000);

function sendButtonClick(button) {
    requestRemote.open('POST', document.URL + 'remote', true);
    requestRemote.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    requestRemote.send('msg=' + button)
}

window.onload = function () {
    getValues();
    const modal = document.getElementById("myModal");
    const btn = document.getElementsByClassName("setting")[0];
    const span = document.getElementsByClassName("close")[0];
    btn.addEventListener('click', function () {
        modal.style.display = "block";
    });
    span.onclick = function () {
        modal.style.display = "none";
    }
    window.onclick = function (event) {
        if (event.target === modal) {
            modal.style.display = "none";
        }
    }
};