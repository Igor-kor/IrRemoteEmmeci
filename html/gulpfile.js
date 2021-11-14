'use strict';
const gulp = require('gulp');
const htmlmin = require('gulp-htmlmin');
const cleanCSS = require('gulp-clean-css');
const twig = require('gulp-twig');
const minify = require('gulp-minify');
const rename = require('gulp-rename');
// Task to minify HTML
gulp.task('minify-html', function() {
    return gulp.src('build/*.html')
        .pipe(htmlmin({ collapseWhitespace: true,removeComments: true }))
        .pipe(gulp.dest('public/'));
});


gulp.task('minify-css', () => {
    return gulp.src('source/*.css')
        .pipe(cleanCSS())
        .pipe(gulp.dest('build/'));
});

gulp.task('compile', function () {
    return gulp.src('source/emmeci.twig')
        .pipe(twig({
            data: {
                title: 'Gulp and Twig',
                benefits: [
                    'Fast',
                    'Flexible',
                    'Secure'
                ]
            }
        }))
        .pipe(gulp.dest('build/'));
});

gulp.task('compile_index_h', function () {
    return gulp.src('source/index_html.twig')
        .pipe(twig({
            data: { }
        }))
        .pipe(rename({ extname: '.h' }))
        .pipe(gulp.dest('../ESP01Remote/')
        );
});

gulp.task('compress',async function() {
    gulp.src('source/*.js')
        .pipe(minify())
        .pipe(gulp.dest('build'));
});

gulp.task('watch', function (){
    gulp.watch('source/*.*',  gulp.parallel('default'));
// other tasks
});

gulp.task('default',gulp.series('compress','minify-css','compile','minify-html','compile_index_h'));