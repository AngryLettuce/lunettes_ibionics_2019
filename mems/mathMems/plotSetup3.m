function [laserplot, normplot, pupilplot] = plotSetup3(Vlm, N, Vmw, mems, wallcorner)
    hold on
    plot3([Vlm(1) 0],[Vlm(2) 0],[Vlm(3) 0]) %laser to MEMS
    laserplot = plot3([0 Vmw(1)],[0 Vmw(2)],[0 Vmw(3)], 'green', 'LineWidth', 2); %MEMS to wall
    pupilplot = plot3(wallcorner(1,1), wallcorner(1,2), wallcorner(1,3));
    normplot  = plot3([0 N(1)],[0 N(2)],[0 N(3)]); %Normal
    surf(mems.X,mems.Y,mems.Z,'FaceColor','white') %mems surface
  
    fill3(wallcorner(:,1)', wallcorner(:,2)', wallcorner(:,3)', 'black') %wall surface
    drawGlasses(0);
    head();

    % adjusting various graph view and options
    grid on
    grid minor
    xlabel('X')
    ylabel('Y')
    zlabel('Z')
    xlim([-150 150])
    ylim([-1250 10])
    daspect([1 1 1])
    view(90,-90)
    title('Vue de face avec mur')  
    set(gca,'visible','off')
    hold off
  
end