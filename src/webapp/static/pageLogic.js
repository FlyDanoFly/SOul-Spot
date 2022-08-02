penguinRequestId = 0;
async function postData(url = '', data = {}) {
    // Default options are marked with *
    console.log('postData' + penguinRequestId);
    data['_requestId'] = penguinRequestId;
    penguinRequestId += 1;
    const response = await fetch(url, {
      method: 'POST', // *GET, POST, PUT, DELETE, etc.
      mode: 'cors', // no-cors, *cors, same-origin
      cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
      credentials: 'same-origin', // include, *same-origin, omit
      headers: {
        'Content-Type': 'application/json'
        // 'Content-Type': 'application/x-www-form-urlencoded',
      },
      redirect: 'follow', // manual, *follow, error
      referrerPolicy: 'no-referrer', // no-referrer, *no-referrer-when-downgrade, origin, origin-when-cross-origin, same-origin, strict-origin, strict-origin-when-cross-origin, unsafe-url
      body: JSON.stringify(data) // body data type must match "Content-Type" header
    });
    return response.status; // parses JSON response into native JavaScript objects
    // return response.json(); // parses JSON response into native JavaScript objects
}

console.log("posting data");
// postData('/ajax', { answer: 42 })
//   .then(data => {
//     console.log(data); // JSON data parsed by `data.json()` call
//   });

if (false) {
    // converting to penguin slider
    Array.from(document.getElementsByClassName('slider-div')).forEach(
        function (theDiv) {
            console.log('outputting');
            console.log(theDiv.getElementsByClassName('slider-textbox')[0]);
        }
    );
}


if (false) {

    console.log('ready2');
    const program = document.getElementsByName("program-mode")[0];
const slider = document.getElementById("slider");
const sliderResult = document.getElementById("slider-result");
function updateIt() {
    sliderResult.textContent = slider.value;
    console.log('hello');
    console.log(program);
    postData('/ajax', {program: program.value, value: slider.value});
}
let intervalId;
const slider1 = slider;
const text1 = sliderResult;
updateIt();

slider1.addEventListener("mouseup", function() {
    clearInterval(intervalId);
    updateIt();
});

slider1.addEventListener("mousedown", function() {
    intervalId = setInterval(() => {
        updateIt();
    }, 100)
});
}