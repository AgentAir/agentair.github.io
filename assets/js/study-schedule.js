const scheduleItems = [
  {
    certification: "AZ-104",
    phase: "参考書1周",
    start: "2026-05-12",
    end: "2026-05-25",
    weekdayTask: "参考書を進める",
    weekendTask: "まとめて読み進める",
    memo: "OneNote作成を目的化しすぎず、問題集に入ることを優先する",
    status: "current",
    color: "book"
  },
  {
    certification: "AZ-104",
    phase: "問題集演習",
    start: "2026-05-26",
    end: "2026-06-21",
    weekdayTask: "15問",
    weekendTask: "35問",
    memo: "週145問ペース。理解が浅い問題だけメモする",
    status: "planned",
    color: "practice"
  },
  {
    certification: "AZ-104",
    phase: "弱点復習・模擬試験",
    start: "2026-06-22",
    end: "2026-06-28",
    weekdayTask: "間違えた問題の復習",
    weekendTask: "模擬試験・総復習",
    memo: "受験日前の最終調整期間",
    status: "planned",
    color: "review"
  },
  {
    certification: "Terraform Associate",
    phase: "基礎復習",
    start: "2026-07-01",
    end: "2026-07-14",
    weekdayTask: "Terraform の基本構文と主要コマンドを復習",
    weekendTask: "既存ラボと問題の復習",
    memo: "AZ-104後の次資格。IaC理解を固める",
    status: "tentative",
    color: "terraform"
  },
  {
    certification: "Terraform Associate",
    phase: "問題演習・ラボ",
    start: "2026-07-15",
    end: "2026-08-09",
    weekdayTask: "問題演習",
    weekendTask: "ラボ構築・弱点復習",
    memo: "既存のTerraformラボ資産を活用する",
    status: "tentative",
    color: "terraform-lab"
  },
  {
    certification: "Terraform Associate",
    phase: "弱点復習・模擬試験",
    start: "2026-08-10",
    end: "2026-08-16",
    weekdayTask: "間違えた問題の復習",
    weekendTask: "模擬試験・総復習",
    memo: "受験前の最終調整期間",
    status: "tentative",
    color: "terraform-review"
  },
  {
    certification: "DEVCOR",
    phase: "基礎復習",
    start: "2026-08-17",
    end: "2026-09-30",
    weekdayTask: "API / Python / Cisco Platform API の復習",
    weekendTask: "ラボと問題演習",
    memo: "Terraform後の次候補。DevNet Expertへつなげる",
    status: "tentative",
    color: "foundation"
  },
  {
    certification: "DEVCOR",
    phase: "問題演習・ラボ",
    start: "2026-10-01",
    end: "2026-11-30",
    weekdayTask: "問題演習",
    weekendTask: "ラボ構築・復習",
    memo: "既存のDEVCORラボ資産を活用する",
    status: "tentative",
    color: "lab"
  },
  {
    certification: "DevNet Expert",
    phase: "ラボ準備",
    start: "2026-12-01",
    end: "2027-02-28",
    weekdayTask: "技術要素の確認",
    weekendTask: "ラボ設計・自動化検証",
    memo: "長期目標。焦らずロードマップ上に置く",
    status: "tentative",
    color: "expert"
  }
];

const nextCertifications = [
  { name: "Terraform Associate", status: "次予定", note: "AZ-104後の次資格としてロードマップに配置" },
  { name: "DEVCOR / DevNet Expert", status: "後続予定", note: "Terraform後の長期ロードマップとして配置" },
  { name: "Network Specialist", status: "候補" }
];

const MS_PER_DAY = 24 * 60 * 60 * 1000;
const timelineModes = {
  "1m": { label: "1ヶ月", months: 1, beforeDays: 7, tick: "week", minWidth: 720 },
  "3m": { label: "3ヶ月", months: 3, beforeDays: 7, tick: "week", minWidth: 980 },
  "6m": { label: "6ヶ月", months: 6, beforeDays: 14, tick: "month", minWidth: 1120 },
  all: { label: "全体", tick: "month", minWidth: 1280 }
};

let currentRangeMode = "all";

function parseDate(value) {
  return new Date(`${value}T00:00:00`);
}

function formatDate(date) {
  return date.toLocaleDateString("ja-JP", {
    year: "numeric",
    month: "2-digit",
    day: "2-digit"
  });
}

function daysBetween(start, end) {
  return Math.round((end - start) / MS_PER_DAY);
}

function addMonths(date, months) {
  const next = new Date(date);
  next.setMonth(next.getMonth() + months);
  return next;
}

function getTimelineRange(items, today, modeKey) {
  const mode = timelineModes[modeKey];
  const dates = items.flatMap((item) => [parseDate(item.start), parseDate(item.end)]);

  if (modeKey === "all") {
    const min = new Date(Math.min(...dates));
    const max = new Date(Math.max(...dates));
    min.setDate(min.getDate() - 27);
    max.setDate(max.getDate() + 7);
    return { min, max, totalDays: daysBetween(min, max) + 1, mode };
  }

  const min = new Date(today);
  min.setDate(min.getDate() - mode.beforeDays);
  const max = addMonths(today, mode.months);
  max.setDate(max.getDate() + 7);

  return { min, max, totalDays: daysBetween(min, max) + 1, mode };
}

function renderSummaryDates(range, today) {
  document.getElementById("schedule-range").textContent =
    `${range.mode.label}: ${formatDate(range.min)} - ${formatDate(range.max)}`;
  document.getElementById("today-label").textContent = formatDate(today);
}

function getTicks(range) {
  const ticks = [];
  const cursor = new Date(range.min);

  if (range.mode.tick === "week") {
    cursor.setDate(cursor.getDate() - cursor.getDay());
    while (cursor <= range.max) {
      if (cursor >= range.min) {
        ticks.push({
          offset: daysBetween(range.min, cursor),
          label: `${cursor.getMonth() + 1}/${cursor.getDate()}`
        });
      }
      cursor.setDate(cursor.getDate() + 7);
    }
    return ticks;
  }

  cursor.setDate(1);
  while (cursor <= range.max) {
    if (cursor >= range.min) {
      ticks.push({
        offset: daysBetween(range.min, cursor),
        label: `${cursor.getFullYear()}/${cursor.getMonth() + 1}`
      });
    }
    cursor.setMonth(cursor.getMonth() + 1);
  }

  return ticks;
}

function getVisibleItems(items, range) {
  return items.filter((item) => {
    const start = parseDate(item.start);
    const end = parseDate(item.end);
    return end >= range.min && start <= range.max;
  });
}

function renderGantt(items, range, today) {
  const chart = document.getElementById("gantt-chart");
  const visibleItems = getVisibleItems(items, range);
  const groups = [...new Set(items.map((item) => item.certification))];
  const ticks = getTicks(range);
  const todayOffset = (daysBetween(range.min, today) / range.totalDays) * 100;
  const showToday = today >= range.min && today <= range.max;

  chart.innerHTML = "";
  chart.style.setProperty("--timeline-days", range.totalDays);
  chart.style.setProperty("--timeline-width", `${range.mode.minWidth}px`);
  chart.style.setProperty("--today-left", `${todayOffset}%`);

  const header = document.createElement("div");
  header.className = "gantt-header";
  header.innerHTML = `<div class="gantt-row-label gantt-corner">資格</div><div class="gantt-timeline-labels"></div>`;
  const labelArea = header.querySelector(".gantt-timeline-labels");

  ticks.forEach((tick) => {
    const label = document.createElement("span");
    label.style.left = `${(tick.offset / range.totalDays) * 100}%`;
    label.textContent = tick.label;
    labelArea.append(label);
  });

  if (showToday) {
    const todayLine = document.createElement("span");
    todayLine.className = "gantt-today-line";
    todayLine.style.left = `${todayOffset}%`;
    labelArea.append(todayLine);
  }

  chart.append(header);

  groups.forEach((group) => {
    const row = document.createElement("div");
    row.className = "gantt-row";

    const label = document.createElement("div");
    label.className = "gantt-row-label";
    label.textContent = group;

    const track = document.createElement("div");
    track.className = "gantt-track";

    if (showToday) {
      const todayLine = document.createElement("span");
      todayLine.className = "gantt-today-line";
      todayLine.style.left = `${todayOffset}%`;
      track.append(todayLine);
    }

    visibleItems
      .filter((item) => item.certification === group)
      .forEach((item) => {
        const start = parseDate(item.start);
        const end = parseDate(item.end);
        const visibleStart = start < range.min ? range.min : start;
        const visibleEnd = end > range.max ? range.max : end;
        const offset = daysBetween(range.min, visibleStart);
        const duration = daysBetween(visibleStart, visibleEnd) + 1;
        const bar = document.createElement("div");
        bar.className = `gantt-bar gantt-${item.color}`;
        if (today >= start && today <= end) {
          bar.classList.add("is-active");
        }
        bar.style.left = `${(offset / range.totalDays) * 100}%`;
        bar.style.width = `max(34px, ${(duration / range.totalDays) * 100}%)`;
        bar.innerHTML = `<span>${item.phase}</span>`;
        bar.title = `${item.certification}: ${item.phase} (${item.start} - ${item.end})`;
        track.append(bar);
      });

    row.append(label, track);
    chart.append(row);
  });
}

function renderRangeControls() {
  document.querySelectorAll(".range-button").forEach((button) => {
    const isActive = button.dataset.range === currentRangeMode;
    button.classList.toggle("is-active", isActive);
    button.setAttribute("aria-pressed", String(isActive));
  });
}

function renderTable(items) {
  const tbody = document.getElementById("schedule-table-body");
  tbody.innerHTML = "";

  items.forEach((item) => {
    const row = document.createElement("tr");
    if (item.status === "current") {
      row.classList.add("current-row");
    }

    [
      item.certification,
      item.phase,
      item.start,
      item.end,
      item.weekdayTask,
      item.weekendTask,
      item.memo
    ].forEach((value, index) => {
      const cell = document.createElement("td");
      cell.textContent = value;
      if (index === 0) {
        cell.classList.add("cert-name-cell");
      }
      row.append(cell);
    });

    tbody.append(row);
  });
}

function renderNextCertifications(items) {
  const container = document.getElementById("next-certifications");
  container.innerHTML = "";

  items.forEach((item) => {
    const card = document.createElement("article");
    card.className = "next-cert-card";
    card.innerHTML = `
      <span class="tag">${item.status}</span>
      <strong>${item.name}</strong>
      ${item.note ? `<small>${item.note}</small>` : ""}
    `;
    container.append(card);
  });
}

function renderSchedulePage() {
  const today = new Date();
  today.setHours(0, 0, 0, 0);
  const range = getTimelineRange(scheduleItems, today, currentRangeMode);

  renderSummaryDates(range, today);
  renderGantt(scheduleItems, range, today);
  renderRangeControls();
  renderTable(scheduleItems);
  renderNextCertifications(nextCertifications);
}

function initSchedulePage() {
  document.querySelectorAll(".range-button").forEach((button) => {
    button.addEventListener("click", () => {
      currentRangeMode = button.dataset.range;
      renderSchedulePage();
    });
  });

  renderSchedulePage();
}

initSchedulePage();
