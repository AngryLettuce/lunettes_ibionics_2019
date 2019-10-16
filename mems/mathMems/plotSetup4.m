function [laserplot, normplot, memsplot] = plotSetup4(Vlm, N, Vmw, mems, wallcorner)

    hold on
    plot3([Vlm(1) 0],[Vlm(2) 0],[Vlm(3) 0]) %laser to MEMS
    laserplot = plot3([0 Vmw(1)],[0 Vmw(2)],[0 Vmw(3)], 'green'); %MEMS to wall
    normplot  = plot3([0 N(1)],[0 N(2)],[0 N(3)]); %Normal
    memsplot = surf(mems.X,mems.Y,mems.Z,'FaceColor','white'); %mems surface
    fill3(wallcorner(:,1)', wallcorner(:,2)', wallcorner(:,3)', 'black') %wall surface

    drawGlasses(0);
%     head();
%     fullbody();

    % adjusting various graph view and options
    grid on
    grid minor
    xlabel('X')
    ylabel('Y')
    zlabel('Z')
    xlim([-50 50])
    ylim([-50 10])
    zlim([-10 50])

    daspect([1 1 1])
    view(90,-40)
    title('Vue des lunettes et du MEMS')
    hold off 
    
end