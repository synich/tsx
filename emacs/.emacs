(menu-bar-mode -1)
;(tool-bar-mode -1)
(fset 'yes-or-no-p 'y-or-n-p)

(setq viper-mode t)
(require 'viper)
(setq line-number-mode t)
(setq column-number-mode t)
(global-font-lock-mode)

(setq kill-whole-line t)
(global-set-key "\C-r" 'enlarge-window)
(global-set-key "\C-p" 'shrink-window)

(add-hook 'gdb-mode-hook
          '(lambda ()  (define-key c-mode-base-map [(f5)] 'gud-go)
		               (define-key c-mode-base-map [(f7)] 'gud-step)
					   (define-key c-mode-base-map [(f8)] 'gud-next)))

;(setq inferior-lisp-program "/bin/clisp")
(add-to-list 'load-path "~/.emacs.d")
;(require 'slime)
;(slime-setup)
;(require 'slime-autoloads)

(autoload 'paredit-mode "paredit"
  "Minor mode for pseudo-structurally editing Lisp code."
  t)

;;;;;;;;;;;;
;; Scheme 
;;;;;;;;;;;;

(require 'cmuscheme)
(setq scheme-program-name "tsx")         ;; 如果用 Petite 就改成 "petite"


;; bypass the interactive question and start the default interpreter
(defun scheme-proc ()
  "Return the current Scheme process, starting one if necessary."
  (unless (and scheme-buffer
               (get-buffer scheme-buffer)
               (comint-check-proc scheme-buffer))
    (save-window-excursion
      (run-scheme scheme-program-name)))
  (or (scheme-get-process)
      (error "No current process. See variable `scheme-buffer'")))


(defun scheme-split-window ()
  (cond
   ((= 1 (count-windows))
    (delete-other-windows)
    (split-window-vertically (floor (* 0.68 (window-height))))
    (other-window 1)
    (switch-to-buffer "*scheme*")
    (other-window 1))
   ((not (find "*scheme*"
               (mapcar (lambda (w) (buffer-name (window-buffer w)))
                       (window-list))
               :test 'equal))
    (other-window 1)
    (switch-to-buffer "*scheme*")
    (other-window -1))))


(defun scheme-send-last-sexp-split-window ()
  (interactive)
  (scheme-split-window)
  (scheme-send-last-sexp))


(defun scheme-send-definition-split-window ()
  (interactive)
  (scheme-split-window)
  (scheme-send-definition))

(add-hook 'scheme-mode-hook
  (lambda ()
    (paredit-mode 1)
    (define-key scheme-mode-map (kbd "<f5>") 'scheme-send-last-sexp-split-window)
    (define-key scheme-mode-map (kbd "<f6>") 'scheme-send-definition-split-window)))

(require 'parenface)
(set-face-foreground 'paren-face "DimGray")
