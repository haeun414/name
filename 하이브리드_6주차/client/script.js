document.addEventListener("DOMContentLoaded", function () {
    const getDataButton = document.getElementById("getDataButton");
    const resultElement = document.getElementById("result");

    getDataButton.addEventListener("click", function () {
        const port = 8080; // 서버 포트 번호
        fetch(`http://localhost:${port}/hi`) // 서버의 hi 함수에 GET 요청을 보냄
            .then((response) => response.json())
            .then((data) => {
                resultElement.innerText = data.message;
            })
            .catch((error) => {
                console.error("Error:", error);
            });
    });
});
