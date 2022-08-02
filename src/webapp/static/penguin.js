

function maxFireRate(func, timeout = 300) {
    let timer = null;
    return (...args) => {
        if (timer) return;
        timer = setTimeout(() => {
            func.apply(this, args);
            timer = null;
        }, timeout);
    }
}

const maxPostData = maxFireRate((input) => {
    const stuffAr = input.id.slice('id-slider-'.length).split('__');
    const obob = {};
    for (let li of document.querySelectorAll('.program-mode-' + stuffAr[0])) {
        const stuffAr2 = li.id.slice('id-slider-'.length).split('__');
        obob[stuffAr2[1]] = li.value;
    }
    const obob1 = {
        'program_mode': stuffAr[0],
        'data': obob 
    };
    postData('/ajax', obob1).then(data => {
        console.log(data); // JSON data parsed by `data.json()` call
    });
});

function inputHandler(event){
    let input = event.target;
    let label = input.labels[0];
    const value = Number(input.value) / Number(input.max);
    input.style.setProperty("--thumb-rotate", `${value * 1440}deg`);
    label.innerHTML = Math.round(input.value);

    maxPostData(input);
}

[...document.querySelectorAll("input")].forEach((input) => {
// for (let i = 0; i < document.querySelectorAll("input").length; i++) {
    // const inputa = document.querySelectorAll("input")[i];
    input.addEventListener("input", inputHandler);
});
