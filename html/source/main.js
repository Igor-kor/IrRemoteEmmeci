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
}, 5000);

function sendButtonClick(button) {
    requestRemote.open('POST', document.URL + 'remote', true);
    requestRemote.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    requestRemote.send('msg=' + button)
}

function restart() {
    requestRemote.open('GET', document.URL + 'restart', true);
    requestRemote.send(null)
}

document.addEventListener("DOMContentLoaded", function(event) {
    getValues();
    const modal = document.getElementById("myModal");
    const modalForm = document.getElementsByClassName("modal-form")[0];
    const btn = document.getElementsByClassName("setting")[0];
    const span = document.getElementsByClassName("close")[0];
    btn.addEventListener('click', function () {
        getSetting();
    });
    span.onclick = function () {
        modal.style.display = "none";
    }
    modalForm.addEventListener("submit",sendSetting);
    window.onclick = function (event) {
        if (event.target === modal) {
            modal.style.display = "none";
        }
    }
    const inputs = document.querySelectorAll(".trim-change");
    inputs.forEach((input)=>{
        input.addEventListener('change',()=>{
            input.value= input.value.trim();
        });
    });
});
const requestGetSettings = new XMLHttpRequest();
const requestSendSettings = new XMLHttpRequest();
async function sendSetting(event) {
    event.preventDefault ();
    event.stopImmediatePropagation ();
    requestSendSettings.open('POST', document.URL + 'setting', true);
    requestSendSettings.send(new FormData(document.querySelector(".modal-form")));
}
requestSendSettings.addEventListener("readystatechange", function (){
    if (requestSendSettings.readyState === 4 && requestSendSettings.status === 200) {
        document.getElementsByClassName("close")[0].click();
    }
    if(requestSendSettings.readyState === 4  && requestSendSettings.status !== 200){
        alert("Error save settings, check connect!");
    }
});

function getSetting(){
    requestGetSettings.open('GET',  document.URL + 'setting', true);
    requestGetSettings.responseType = 'json';
    requestGetSettings.send(null);
}
requestGetSettings.addEventListener("readystatechange", function (){
    const modal = document.getElementById("myModal");
    const modalForm = document.querySelector(".modal-form");
    if (requestGetSettings.readyState === 4 && requestGetSettings.status === 200) {
        modalForm.APSSID.value = requestGetSettings.response.APSSID;
        modalForm.APPassword.value = requestGetSettings.response.APPassword;
        modalForm.APIpAddress.value = requestGetSettings.response.APIpAddress;
        modalForm.APGateway.value = requestGetSettings.response.APGateway;
        modalForm.APSubnet.value = requestGetSettings.response.APSubnet;
        modalForm.ClientSSID.value = requestGetSettings.response.ClientSSID;
        modalForm.ClientPassword.value = requestGetSettings.response.ClientPassword;
        modalForm.TShift.value = requestGetSettings.response.TShift;
        modalForm.APChannel.value = requestGetSettings.response.APChannel;
        modalForm.APHidden.value = requestGetSettings.response.APHidden;
        modalForm.APMaxConnection.value = requestGetSettings.response.APMaxConnection;
        modal.style.display = "block";
    }
    if(requestGetSettings.readyState === 4  && requestGetSettings.status !== 200){
        alert("Error load settings, check connect!");
    }
});