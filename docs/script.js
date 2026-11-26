// === 1. OFFLINE SOUND EFFECTS (Web Audio API, like sounds.cpp) ===
let audioCtx = null;
let soundEnabled = true;

function getAudioCtx() {
  if (!audioCtx) {
    const AudioContextClass = window.AudioContext || window.webkitAudioContext;
    if (AudioContextClass) audioCtx = new AudioContextClass();
  }
  return audioCtx;
}

function playTone(freq, durationMs, delayMs = 0) {
  if (!soundEnabled) return;
  const ctx = getAudioCtx();
  if (!ctx) return;

  const startTime = ctx.currentTime + delayMs / 1000;
  const osc = ctx.createOscillator();
  const gain = ctx.createGain();

  osc.type = "square";
  osc.frequency.setValueAtTime(freq, startTime);

  gain.gain.setValueAtTime(0.08, startTime);
  gain.gain.exponentialRampToValueAtTime(0.001, startTime + durationMs / 1000);

  osc.connect(gain);
  gain.connect(ctx.destination);

  osc.start(startTime);
  osc.stop(startTime + durationMs / 1000);
}

const playMoveSound = (isAI) => playTone(isAI ? 480 : 650, 70);
const playWinSound = () => {
  playTone(523, 100, 0);
  playTone(659, 100, 105);
  playTone(784, 150, 210);
  playTone(1046, 250, 365);
};
const playLoseSound = () => {
  playTone(400, 150, 0);
  playTone(350, 150, 155);
  playTone(300, 300, 310);
};
const playDrawSound = () => {
  playTone(440, 120, 0);
  playTone(440, 120, 160);
};
const playUltimateIntroSound = () => {
  playTone(330, 100, 0);
  playTone(440, 100, 105);
  playTone(554, 120, 210);
  playTone(659, 220, 335);
};
const playBattleMoveSound = (isAI) => {
  playTone(isAI ? 380 : 740, 55, 0);
  playTone(isAI ? 320 : 880, 45, 60);
};
const playBoardCaptureSound = () => {
  playTone(587, 90, 0);
  playTone(740, 90, 95);
  playTone(988, 180, 190);
};

// === 2. GAME LOGIC (like logic.cpp) ===
const WIN_LINES = [
  [0, 1, 2], [3, 4, 5], [6, 7, 8],
  [0, 3, 6], [1, 4, 7], [2, 5, 8],
  [0, 4, 8], [2, 4, 6]
];

const isOccupied = (board, pos) => board[pos] === "X" || board[pos] === "O";
const isBoardFull = (board) => board.every((_, i) => isOccupied(board, i));
const isMainCellResolved = (mainBoard, pos) =>
  mainBoard[pos] === "X" || mainBoard[pos] === "O" || mainBoard[pos] === "D";

function checkWin(board, player) {
  return WIN_LINES.some(([a, b, c]) =>
    board[a] === player && board[b] === player && board[c] === player
  );
}

function findWinningMove(board, player) {
  for (const line of WIN_LINES) {
    let count = 0;
    let empty = -1;
    for (const pos of line) {
      if (board[pos] === player) count++;
      else if (!isOccupied(board, pos)) empty = pos;
    }
    if (count === 2 && empty !== -1) return empty;
  }
  return -1;
}

function getAIMove(board, ai, human) {
  let move = findWinningMove(board, ai);
  if (move !== -1) return move;

  move = findWinningMove(board, human);
  if (move !== -1) return move;

  if (!isOccupied(board, 4)) return 4;

  const openCorners = [0, 2, 6, 8].filter((c) => !isOccupied(board, c));
  if (openCorners.length > 0) {
    return openCorners[Math.floor(Math.random() * openCorners.length)];
  }

  const remaining = [];
  for (let i = 0; i < 9; i++) {
    if (!isOccupied(board, i)) remaining.push(i);
  }
  return remaining[Math.floor(Math.random() * remaining.length)];
}

function getUltimateAIMove(mainBoard, subBoards, activeBoard, ai, human) {
  let targetBoard = activeBoard;

  if (targetBoard === -1 || isMainCellResolved(mainBoard, targetBoard)) {
    const openBoards = [];
    for (let b = 0; b < 9; b++) {
      if (!isMainCellResolved(mainBoard, b)) openBoards.push(b);
    }

    targetBoard = openBoards.find((b) => findWinningMove(subBoards[b], ai) !== -1) ?? -1;
    if (targetBoard === -1) {
      targetBoard = openBoards.find((b) => findWinningMove(subBoards[b], human) !== -1) ?? -1;
    }
    if (targetBoard === -1) {
      targetBoard = !isMainCellResolved(mainBoard, 4)
        ? 4
        : openBoards[Math.floor(Math.random() * openBoards.length)];
    }
  }

  const targetCell = getAIMove(subBoards[targetBoard], ai, human);
  return { targetBoard, targetCell };
}

// === 3. JOKES & TYPEWRITER (like display.cpp) ===
const JOKES = [
  ["Why do programmers wear glasses?", "Because they can't C#!"],
  ["How many programmers to change a light bulb?", "None - hardware problem."],
  ["Programmer's favorite hangout?", "Foo Bar."],
  ["Why did the dev quit?", "Didn't get arrays!"]
];

let typewriterTimers = [];
function clearTypewriters() {
  typewriterTimers.forEach(clearTimeout);
  typewriterTimers = [];
}

function typeText(el, text, delayMs = 30, startDelayMs = 0) {
  el.textContent = "";
  for (let i = 0; i < text.length; i++) {
    const t = setTimeout(() => {
      el.textContent += text[i];
    }, startDelayMs + i * delayMs);
    typewriterTimers.push(t);
  }
}

// === 4. GAME STATE & UI ===
const state = {
  isUltimate: false,
  vsAI: false,
  current: "X",
  gameOver: false,
  aiThinking: false,
  // Normal mode
  board: [],
  // Ultimate mode
  mainBoard: [],
  subBoards: [],
  activeBoard: -1
};

const el = {
  soundToggle: document.getElementById("soundToggle"),
  menuType: document.getElementById("menuType"),
  praiseBanner: document.getElementById("praiseBanner"),
  praiseText: document.getElementById("praiseText"),
  menuOpponent: document.getElementById("menuOpponent"),
  gameScreen: document.getElementById("gameScreen"),
  statusBar: document.getElementById("statusBar"),
  subStatus: document.getElementById("subStatus"),
  boardContainer: document.getElementById("boardContainer"),
  endModal: document.getElementById("endModal"),
  endArt: document.getElementById("endArt"),
  endTitle: document.getElementById("endTitle"),
  jokeQ: document.getElementById("jokeQ"),
  jokeA: document.getElementById("jokeA")
};

function showMenus() {
  clearTypewriters();
  el.endModal.classList.add("hidden");
  el.gameScreen.classList.add("hidden");
  el.menuOpponent.classList.add("hidden");
  el.praiseBanner.classList.add("hidden");
  el.menuType.classList.remove("hidden");
}

function selectGameType(ultimate) {
  state.isUltimate = ultimate;
  el.menuType.classList.add("hidden");
  el.menuOpponent.classList.remove("hidden");

  if (ultimate) {
    playUltimateIntroSound();
    el.praiseBanner.classList.remove("hidden");
    typeText(
      el.praiseText,
      "A true strategist enters the arena! Respect for choosing Ultimate Mode - prepare for battle!",
      22
    );
  } else {
    el.praiseBanner.classList.add("hidden");
  }
}

function startGame(vsAI) {
  state.vsAI = vsAI;
  state.current = "X";
  state.gameOver = false;
  state.aiThinking = false;
  state.board = Array(9).fill("");
  state.mainBoard = Array(9).fill("");
  state.subBoards = Array.from({ length: 9 }, () => Array(9).fill(""));
  state.activeBoard = -1;

  el.menuOpponent.classList.add("hidden");
  el.praiseBanner.classList.add("hidden");
  el.endModal.classList.add("hidden");
  el.gameScreen.classList.remove("hidden");

  render();
}

function updateStatus() {
  if (state.aiThinking) {
    el.statusBar.innerHTML = `<span class="o-color">AI (O)</span> is thinking...`;
  } else {
    const cls = state.current === "X" ? "x-color" : "o-color";
    el.statusBar.innerHTML = `Player <span class="${cls}">${state.current}</span>'s Turn`;
  }

  if (!state.isUltimate) {
    el.subStatus.textContent = state.vsAI ? "Normal Mode - vs AI" : "Normal Mode - Player vs Player";
  } else if (state.activeBoard === -1) {
    el.subStatus.textContent = "Active Board: ANY OPEN BOARD";
  } else {
    el.subStatus.textContent = `Active Board: #${state.activeBoard + 1}`;
  }
}

function render() {
  updateStatus();
  el.boardContainer.innerHTML = "";

  if (!state.isUltimate) {
    const grid = document.createElement("div");
    grid.className = "normal-board";

    state.board.forEach((val, idx) => {
      const btn = document.createElement("button");
      btn.className = `cell ${val.toLowerCase()}`;
      btn.textContent = val || (idx + 1);
      if (!val) btn.style.color = "var(--muted)";
      btn.disabled = state.gameOver || state.aiThinking || isOccupied(state.board, idx);
      btn.addEventListener("click", () => handleNormalMove(idx));
      grid.appendChild(btn);
    });

    el.boardContainer.appendChild(grid);
  } else {
    const grid = document.createElement("div");
    grid.className = "ultimate-board";

    for (let b = 0; b < 9; b++) {
      const mini = document.createElement("div");
      const isActive = !state.gameOver && !isMainCellResolved(state.mainBoard, b) &&
        (state.activeBoard === -1 || state.activeBoard === b);
      mini.className = `mini-board${isActive ? " active" : ""}`;

      for (let c = 0; c < 9; c++) {
        const val = state.subBoards[b][c];
        const cell = document.createElement("button");
        cell.className = `mini-cell ${val.toLowerCase()}`;
        cell.textContent = val || (c + 1);
        cell.disabled = !isActive || state.aiThinking || isOccupied(state.subBoards[b], c);
        cell.addEventListener("click", () => handleUltimateMove(b, c));
        mini.appendChild(cell);
      }

      const owner = state.mainBoard[b];
      if (owner) {
        const overlay = document.createElement("div");
        overlay.className = `mini-overlay ${owner.toLowerCase()}`;
        overlay.textContent = owner === "D" ? "DRAW" : owner;
        mini.appendChild(overlay);
      }

      grid.appendChild(mini);
    }

    el.boardContainer.appendChild(grid);
  }
}

function handleNormalMove(pos) {
  if (state.gameOver || state.aiThinking || isOccupied(state.board, pos)) return;

  const isAI = state.vsAI && state.current === "O";
  state.board[pos] = state.current;
  playMoveSound(isAI);

  if (checkWin(state.board, state.current)) {
    state.gameOver = true;
    render();
    showEndModal(state.current, isAI);
    return;
  }

  if (isBoardFull(state.board)) {
    state.gameOver = true;
    render();
    showEndModal(null, false);
    return;
  }

  state.current = state.current === "X" ? "O" : "X";
  render();

  if (state.vsAI && state.current === "O") {
    state.aiThinking = true;
    render();
    setTimeout(() => {
      state.aiThinking = false;
      const aiMove = getAIMove(state.board, "O", "X");
      handleNormalMove(aiMove);
    }, 550);
  }
}

function handleUltimateMove(b, c) {
  if (state.gameOver || state.aiThinking) return;
  if (isMainCellResolved(state.mainBoard, b) || isOccupied(state.subBoards[b], c)) return;
  if (state.activeBoard !== -1 && state.activeBoard !== b) return;

  const isAI = state.vsAI && state.current === "O";
  state.subBoards[b][c] = state.current;

  if (checkWin(state.subBoards[b], state.current)) {
    state.mainBoard[b] = state.current;
    playBoardCaptureSound();
  } else if (isBoardFull(state.subBoards[b])) {
    state.mainBoard[b] = "D";
    playBattleMoveSound(isAI);
  } else {
    playBattleMoveSound(isAI);
  }

  if (checkWin(state.mainBoard, state.current)) {
    state.gameOver = true;
    render();
    showEndModal(state.current, isAI);
    return;
  }

  const allResolved = state.mainBoard.every((_, i) => isMainCellResolved(state.mainBoard, i));
  if (allResolved) {
    state.gameOver = true;
    render();
    showEndModal(null, false);
    return;
  }

  state.activeBoard = isMainCellResolved(state.mainBoard, c) ? -1 : c;
  state.current = state.current === "X" ? "O" : "X";
  render();

  if (state.vsAI && state.current === "O") {
    state.aiThinking = true;
    render();
    setTimeout(() => {
      state.aiThinking = false;
      const { targetBoard, targetCell } = getUltimateAIMove(
        state.mainBoard, state.subBoards, state.activeBoard, "O", "X"
      );
      handleUltimateMove(targetBoard, targetCell);
    }, 650);
  }
}

function showEndModal(winner, aiWon) {
  clearTypewriters();
  el.endModal.classList.remove("hidden");

  if (!winner) {
    playDrawSound();
    el.endArt.textContent = "🤝";
    el.endTitle.textContent = state.isUltimate
      ? "Ultimate Stalemate! Nobody claimed the main board."
      : "It's a Draw! Nobody wins this round.";
  } else if (aiWon) {
    playLoseSound();
    el.endArt.textContent = "🤖";
    el.endTitle.innerHTML = `The AI (<span class="o-color">${winner}</span>) outsmarted you this time!`;
  } else {
    playWinSound();
    el.endArt.textContent = "🏆";
    const cls = winner === "X" ? "x-color" : "o-color";
    el.endTitle.innerHTML = `Congrats, Player <span class="${cls}">${winner}</span>! You won!`;
  }

  const [q, a] = JOKES[Math.floor(Math.random() * JOKES.length)];
  typeText(el.jokeQ, q, 30, 0);
  typeText(el.jokeA, a, 35, q.length * 30 + 600);
}

// === 5. BUTTON LISTENERS ===
el.soundToggle.addEventListener("click", () => {
  soundEnabled = !soundEnabled;
  el.soundToggle.textContent = soundEnabled ? "Sound: ON" : "Sound: OFF";
});

document.getElementById("btnNormal").addEventListener("click", () => selectGameType(false));
document.getElementById("btnUltimate").addEventListener("click", () => selectGameType(true));
document.getElementById("btnBackToType").addEventListener("click", showMenus);
document.getElementById("btnPvP").addEventListener("click", () => startGame(false));
document.getElementById("btnAI").addEventListener("click", () => startGame(true));
document.getElementById("btnRestart").addEventListener("click", () => startGame(state.vsAI));
document.getElementById("btnChangeMode").addEventListener("click", showMenus);
document.getElementById("btnPlayAgain").addEventListener("click", () => startGame(state.vsAI));
document.getElementById("btnModalMenu").addEventListener("click", showMenus);
