<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Space Invaders</title>
    <style>
        canvas {
            display: block;
            margin: 0 auto;
            background: black;
        }
        #winMessage, #nextLevelButton {
            display: none;
            text-align: center;
            color: white;
            font-size: 48px;
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
        }
    </style>
</head>
<body>
    <canvas id="gameCanvas" width="800" height="600"></canvas>
    <div id="winMessage">You Win!</div>
    <button id="nextLevelButton" onclick="nextLevel()">Next Level</button>
    <script>
        const canvas = document.getElementById('gameCanvas');
        const context = canvas.getContext('2d');
        const winMessage = document.getElementById('winMessage');
        const nextLevelButton = document.getElementById('nextLevelButton');

        const playerWidth = 50;
        const playerHeight = 20;
        const playerSpeed = 5;
        let playerX = canvas.width / 2 - playerWidth / 2;

        const bulletWidth = 5;
        const bulletHeight = 10;
        const bulletSpeed = 7;
        const bullets = [];

        const alienWidth = 40;
        const alienHeight = 20;
        const alienSpeed = 1;
        const alienRows = 3;
        const alienCols = 10;
        let level = 1;
        let aliens = [];

        function initializeAliens() {
            aliens = [];
            for (let row = 0; row < alienRows; row++) {
                for (let col = 0; col < alienCols; col++) {
                    aliens.push({
                        x: 100 + col * (alienWidth + 10),
                        y: 50 + row * (alienHeight + 10),
                        width: alienWidth,
                        height: alienHeight,
                        alive: true
                    });
                }
            }
        }

        initializeAliens(); // Initialize aliens for the first level

        let alienDirection = 1;
        let gameWon = false;

        document.addEventListener('keydown', (event) => {
            if (event.key === 'ArrowLeft') {
                playerX -= playerSpeed;
            } else if (event.key === 'ArrowRight') {
                playerX += playerSpeed;
            } else if (event.key === ' ') {
                bullets.push({
                    x: playerX + playerWidth / 2 - bulletWidth / 2,
                    y: canvas.height - playerHeight - bulletHeight,
                    width: bulletWidth,
                    height: bulletHeight
                });
            }
        });

        function update() {
            if (gameWon) return;

            // Update bullets
            for (let i = 0; i < bullets.length; i++) {
                bullets[i].y -= bulletSpeed;
                if (bullets[i].y + bulletHeight < 0) {
                    bullets.splice(i, 1);
                    i--;
                }
            }

            // Update aliens
            let changeDirection = false;
            for (let i = 0; i < aliens.length; i++) {
                if (!aliens[i].alive) continue;
                aliens[i].x += alienSpeed * alienDirection;
                if (aliens[i].x <= 0 || aliens[i].x + alienWidth >= canvas.width) {
                    changeDirection = true;
                }
            }

            if (changeDirection) {
                alienDirection *= -1;
                for (let i = 0; i < aliens.length; i++) {
                    aliens[i].y += alienHeight;
                }
            }

            // Check collisions
            for (let i = 0; i < bullets.length; i++) {
                for (let j = 0; j < aliens.length; j++) {
                    if (!aliens[j].alive) continue;
                    if (bullets[i].x < aliens[j].x + aliens[j].width &&
                        bullets[i].x + bullets[i].width > aliens[j].x &&
                        bullets[i].y < aliens[j].y + aliens[j].height &&
                        bullets[i].y + bullets[i].height > aliens[j].y) {
                        aliens[j].alive = false;
                        bullets.splice(i, 1);
                        i--;
                        break;
                    }
                }
            }

            // Check for win
            gameWon = aliens.every(alien => !alien.alive);
            if (gameWon) {
                winMessage.style.display = 'block';
                nextLevelButton.style.display = 'block';
            }
        }

        function draw() {
            context.clearRect(0, 0, canvas.width, canvas.height);

            // Draw player
            context.fillStyle = 'blue';
            context.fillRect(playerX, canvas.height - playerHeight, playerWidth, playerHeight);

            // Draw bullets
            context.fillStyle = 'red';
            for (let i = 0; i < bullets.length; i++) {
                context.fillRect(bullets[i].x, bullets[i].y, bullets[i].width, bullets[i].height);
            }

            // Draw aliens
            context.fillStyle = 'green';
            for (let i = 0; i < aliens.length; i++) {
                if (aliens[i].alive) {
                    context.fillRect(aliens[i].x, aliens[i].y, aliens[i].width, aliens[i].height);
                }
            }
        }

        function gameLoop() {
            update();
            draw();
            requestAnimationFrame(gameLoop);
        }

        function nextLevel() {
            level++;
            winMessage.style.display = 'none';
            nextLevelButton.style.display = 'none';
            initializeAliens();
            gameWon = false;
        }

        gameLoop();
    </script>
</body>
</html>