const navMenu = document.getElementById('nav-menu');
const navToggle = document.getElementById('nav-toggle');
const navClose = document.getElementById('nav-close');
if (navToggle) {
    navToggle.addEventListener('click', () => {
        navMenu.classList.add('show-menu');
    });
};
if (navClose) {
    navClose.addEventListener('click', () => {
        navMenu.classList.remove('show-menu');
    });
};
//-----
const navLink = document.querySelectorAll('.nav__link');
const linkAction = () => {
    const navMenu = document.getElementById('nav-menu');
    navMenu.classList.remove('show-menu');
}
navLink.forEach(i => i.addEventListener('click', linkAction));
//-----
const scrollHeader = () => {
    const header = document.getElementById('header');
    if (this.scrollY >= 50) header.classList.add('bg-header');
    else header.classList.remove('bg-header');
}
window.addEventListener('scroll', scrollHeader);
//-----
const sections = document.querySelectorAll('section[id]');
const scrollActive = () => {
    const scrollY = window.pageYOffset;
    sections.forEach(i => {
        const sectionHeight = i.offsetHeight;
        const sectionTop = i.offsetTop - 58; 
        const sectionId = i.getAttribute('id');
        const sectionsClass = document.querySelector('.nav__menu a[href*=' + sectionId + ']');
        if (scrollY > sectionTop && scrollY <= sectionTop + sectionHeight) sectionsClass.classList.add('active-link');
        else sectionsClass.classList.remove('active-link');
    });
}
window.addEventListener('scroll', scrollActive);
//-----
const scrollUp = () => {
    const scrollUp = document.getElementById('scroll-up');
    if (this.scrollY >= 350) scrollUp.classList.add('show-scroll');
    else scrollUp.classList.remove('show-scroll');
}
window.addEventListener('scroll', scrollUp);
//-----
const sr = ScrollReveal({
    origin: 'top',
    distance: '60px',
    duration: 2500,
    delay: 400,
})
sr.reveal(`.home__data, .footer__content, .footer__logo, .footer__description`);
sr.reveal(`.home__img`, { delay: 800 });
sr.reveal(`.features__card, .rules__card, .usage__card`, { interval: 100 });
sr.reveal(`.game__img, .game__data`, { origin: 'left' });